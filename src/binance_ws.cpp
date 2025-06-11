#include "binancecpp/binance_ws.h"

namespace binance {

    void BinanceWsClient::setLocalIP(const std::string localIP) {
        this->localIP = localIP;
    }
    void BinanceWsClient::setRemoteIP(const std::string remoteIP) {
        this->remoteIP = remoteIP;
    }
    void BinanceWsClient::setMessageChannel(shared_ptr<moodycamel::ConcurrentQueue<std::string>> messageChannel) {
        this->messageChannel = messageChannel;
    }
    void BinanceWsClient::setMessageCallback(WS_CB customCallback) {
        this->customCallback = customCallback;
    }
    void BinanceWsClient::init(const std::string apiKey, const std::string secretKey, MarketType marketType, bool useInternal, bool useCombine, bool isTrading) {
        this->apiKey = apiKey;
        this->secretKey = secretKey;
        this->marketType = marketType;
        this->useInternal = useInternal;
        this->useCombine = useCombine;
        this->useTrading = isTrading;
        if (isTrading) {
            this->wsEndpoint = getWsTradingEndpoint(marketType, useInternal);
        } else {
            if (useCombine) {
                this->wsEndpoint = getWsCombineBaseEndpoint(marketType, useInternal);
            } else {
                this->wsEndpoint = getWsBaseEndpoint(marketType, useInternal);
            }
        }

        if (secretKey != "") {
            this->parsedSecretKey = parse_private_key(secretKey);
        }

        this->isConnected = false;
        this->isLogoned = false;

        // this->lock = std::unique_lock<std::mutex>(this->mutex, std::defer_lock);
    }

    std::pair<bool, string> BinanceWsClient::connect_endpoint(std::string& handshakePath) {
        // Implementation of connecting to the WebSocket endpoint
        // This is where you would set up the WebSocket connection and handle messages

        // Step 1: Create a secure TLS connection
        int socket_fd = 0;
        SSL* ssl = nullptr;
        try {
            ssl = create_tls_connection(socket_fd, this->wsEndpoint.first, this->wsEndpoint.second, this->remoteIP, this->localIP);
        } catch (std::exception &e) {
            return std::pair<bool, string> (false, "fail to create tls connection: " + std::string(e.what()));
        }

        this->socketFd = socket_fd;
        this->ssl = ssl;

        // Step 2: Perform the WebSocket handshake
        try {
            perform_websocket_handshake(this->ssl, this->wsEndpoint.first, handshakePath);
        } catch (std::exception &e) {
            return std::pair<bool, string> (false, "fail to perform ws handshake: " + std::string(e.what()));
        }

        this->isConnected = true;
        return std::pair<bool, string>(true, "");
    }

    std::pair<bool, string> BinanceWsClient::send_session_logon() {

        // TODO If original sessionID is not empty, maybe need logout firstly

        // Make payload
        uint64_t timestamp = get_current_ms_epoch();
        std::string logonID = generate_uuid();
        std::string query = ("apiKey=");
        query.append(this->apiKey);
        query.append("&").append("timestamp=").append(std::to_string(timestamp));

        Json::Value paramsJson;
        paramsJson["apiKey"] = this->apiKey;
        paramsJson["timestamp"] = Json::UInt64(timestamp);
        try {
            std::string signature = sign_payload_by_ed25519(this->parsedSecretKey, query);
            paramsJson["signature"] = signature;
        } catch (std::exception &e) {
            return std::pair<bool, string> (false, "fail to logon signature: " + std::string(e.what()));
        }

        Json::Value reqJson;
        reqJson["id"] = logonID;
        reqJson["method"] = "session.logon";
        reqJson["params"] = paramsJson;
        std::string payload = serialize_json_value(reqJson);

        // std::cout << "logon payload: " << payload << std::endl;

        WebSocketPacket packet;
        packet.set_fin(1);
        packet.set_opcode(WebSocketPacket::WSOpcode_Text);
        packet.set_mask(1);
        ByteBuffer messageBuffer;
        packet.set_payload(payload.c_str(), payload.length());
        packet.pack_dataframe(messageBuffer);
        int writeLength = SSL_write(ssl, messageBuffer.bytes(),  messageBuffer.length());
        if (writeLength <= 0) {
            return std::pair<bool, string> (false, "failed to send logon frame");
        }

        char buffer[1024];
        int len = SSL_read(this->ssl, buffer, sizeof(buffer) - 1);
        if (len <= 20) {
            // Maybe read a timestamp data firstly, so read once more
            std::memset(buffer, 0, sizeof(buffer));
            len = SSL_read(this->ssl, buffer, sizeof(buffer) - 1);
        }
        if (len < 0) {
            return std::pair<bool, string> (false, "failed to receive logon response");
        } else {
            // Remove start opcode
            std::memmove(buffer, buffer + 4, len - 4 + 1);
            // std::cout << "logon resp: " << len << std::string(buffer, len-4) << std::endl;
            // Parse logon result
            Json::Value json_result;
            Json::Reader reader;
            json_result.clear();
            reader.parse(std::string(buffer, len).c_str(), json_result);

            int status = 0;
            std::string respID;
            if (json_result.isMember("status")) {
                status = json_result["status"].asInt();
            }
            if (json_result.isMember("id")) {
                respID = json_result["id"].asString();
            }

            if (status == 200 && respID == logonID) {
                this->sessionID = logonID;
                this->isLogoned = true;
                return std::pair<bool, string> (true, "");
            } else {
                this->sessionID = "";
                this->isLogoned = false;
                return std::pair<bool, string> (false, std::string(buffer, len-4));
            }
        }
    }

    std::pair<bool, string> BinanceWsClient::send_subscribe(std::string& payload) {

        if (payload.length() == 0) {
            return std::pair<bool, string>(false, "subscribe payload is empty");
        }

        WebSocketPacket packet;
        packet.set_fin(1);
        packet.set_opcode(WebSocketPacket::WSOpcode_Text);
        packet.set_mask(1);
        ByteBuffer messageBuffer;
        packet.set_payload(payload.c_str(), payload.length());
        packet.pack_dataframe(messageBuffer);
        int writeLength = SSL_write(ssl, messageBuffer.bytes(),  messageBuffer.length());
        // int writeLength = SSL_write(this->ssl, subscribeFrame.c_str(), subscribeFrame.size());
        if (writeLength <= 0) {
            return std::pair<bool, string>(false, "failed to send subscribe frame");
        }

        char buffer[1024];
        int len = SSL_read(this->ssl, buffer, sizeof(buffer) - 1);
        if (len < 0) {
            return std::pair<bool, string>(false, "failed to receive subscribe response");
        } else {
            return std::pair<bool, string>(true, "");
        }
    }

    std::pair<bool, string> BinanceWsClient::start_event_loop() {

        std::string error;

        while(true) {
            if (!this->isConnected) {
                error = "Has closed by outer handler";
                break;
            }
            try {
                int total_recv_len = 0;
                int max_buffer_size = 1024;
                char buffer[max_buffer_size];
                int read_len = 0;
                int pendding_len = 0;
                int recv_start_len = this->recvBuffer.length();
                do {
                    read_len = SSL_read(this->ssl, buffer, sizeof(buffer) - 1);
                    if (read_len > 0) {
                        this->recvBuffer.append(buffer, read_len);
                        total_recv_len += read_len;
                    } else {
                        break;
                    }
                    pendding_len = SSL_pending(this->ssl);
                } while (this->isConnected && pendding_len > 0);

                if (read_len == 0) {
                    error = "Connection closed by peer (gracefully)";
                    break;
                } else if (read_len < 0) {
                    int error_code = SSL_get_error(this->ssl, read_len);
                    ERR_clear_error();
                    if (error_code == SSL_ERROR_WANT_READ || error_code == SSL_ERROR_WANT_WRITE) {
                        // break;
                        // TODO may got this code while connection is closed
                        continue;
                    } else if (error_code == SSL_ERROR_ZERO_RETURN) {
                        error = "Connection closed by peer (gracefully)";
                        break;
                    } else if (error_code == SSL_ERROR_SYSCALL) {
                        error = "I/O error occurred: " + std::string(strerror(errno));
                        break;
                    } else if (error_code == SSL_ERROR_SSL) {
                        error = "SSL protocol error: " + std::string(ERR_reason_error_string(ERR_get_error()));
                        break;
                    } else {
                        error = "Unknown error occurred in SSL_read.";
                        break;
                    }
                }

                if (total_recv_len <= 0) {
                    error = "no data read from ssl connection";
                    break;
                }

                if (this->recvBuffer.length() > recv_start_len && this->recvBuffer.length() > this->recvBuffer.getoft() + WEBSOCKET_HEADER_MIN_SIZE) {
                    // parse the message
                    WebSocketPacket ws_packet;
                    while (true) {
                        uint32_t offset = ws_packet.recv_dataframe(this->recvBuffer);
                        if (offset == 0) {
                            // Not enough data to process, break the loop
                            break;
                        }
                        ByteBuffer &payload = ws_packet.get_payload();
                        this->msgBuffer.append(payload.bytes(), payload.length());
                        if (ws_packet.get_fin() == 1) {
                            // std::cout << "Received WebSocket Message: " << std::string(this->msgBuffer.bytes(), this->msgBuffer.length()) << std::endl;
                            // Process one complete message
                            this->process_one_message(ws_packet, this->msgBuffer);
                            this->msgBuffer.clear();
                        }

                        if (this->recvBuffer.getoft() + WEBSOCKET_HEADER_MIN_SIZE >= this->recvBuffer.length()) {
                            // No more data to process, break the packet parsing loop and continue reading
                            break;
                        }
                        ws_packet.get_payload().clear();
                    }

                    this->recvBuffer.erase(this->recvBuffer.getoft());
                    this->recvBuffer.resetoft();
                } else {
                    // No complete message received yet
                    continue;
                }
            } catch ( exception &e ) {
                // std::cout << "Read exception: " << e.what() << std::endl;
                error = "loop read exception: " + std::string(e.what());
                break;
            }
        }

        this->release_resource();

        return std::pair<bool, string>(false, error);
    }

    std::pair<bool, string> BinanceWsClient::process_one_message(WebSocketPacket& packet, ByteBuffer& mssageBuffer) {

        if (packet.get_opcode() == WebSocketPacket::WSOpcode_Ping) {
            // Handle Ping
            // Send Pong response
            if (ssl == nullptr) {
                return std::pair<bool, string>(false, "ssl is null while send pong package");
            }
            packet.set_fin(1);
            packet.set_opcode(WebSocketPacket::WSOpcode_Pong);
            packet.set_mask(1);
            ByteBuffer pong_buf;
            packet.pack_dataframe(pong_buf);
            int writeLength;
            string writeError;

            // lock before write data
            std::unique_lock<std::shared_mutex> lock(this->rw_lock);
            // this->lock.lock();
            try {
                writeLength = SSL_write(ssl, pong_buf.bytes(), pong_buf.length());
            } catch (std::exception &e) {
                writeError = e.what();
            }
            // this->lock.unlock();
            if (writeLength <= 0) {
                return std::pair<bool, string>(false, "failed to write pong package: " + writeError);
            }
        } else if (packet.get_opcode() == WebSocketPacket::WSOpcode_Continue) {
            // Handle continuation frame
        }
        if (packet.get_opcode() == WebSocketPacket::WSOpcode_Text) {
            // Parse to json result
            // Json::Value json_result;
            // Json::Reader reader;
            // json_result.clear();
            // reader.parse(std::string(mssageBuffer.bytes(), mssageBuffer.length()).c_str() , json_result);

            // Cumstomer could choose Json library
            std::string messageJson = std::string(mssageBuffer.bytes(), mssageBuffer.length()).c_str();

            if (this->messageChannel == nullptr && this->customCallback == nullptr) {
                std::cout << "require message-channel or custom callback for consume: " << messageJson << std::endl;
            } else {
                // Send json to channel
                if (this->messageChannel != nullptr) {
                    bool result = this->messageChannel->try_enqueue(messageJson);
                    if (!result) {
                        std::cout << "can not enqueue item: " << messageJson << std::endl;
                    }
                }

                // Process json result by custom
                if (this->customCallback != nullptr) {
                    (*this->customCallback)(messageJson);
                }
            }
        }
\
        return std::pair<bool, string>(true, "");
    }

    void BinanceWsClient::stop() {

        std::unique_lock<std::shared_mutex> lock(this->rw_lock);
        if (this->socketFd > 0) {
            try {
                close(this->socketFd);
                this->socketFd = 0;
            } catch (std::exception &e) {}
        }
        if (this->ssl != nullptr && SSL_get_shutdown(this->ssl) != SSL_SENT_SHUTDOWN) {
            try {
                SSL_shutdown(this->ssl);
            } catch (std::exception &e) {}
        }
        this->isConnected = false;
        this->isLogoned = false;
    }

    void BinanceWsClient::free() {

        std::unique_lock<std::shared_mutex> lock(this->rw_lock);
        if (this->messageChannel != nullptr) {
            // clear channel data
            string temp;
            while (this->messageChannel->try_dequeue(temp)) {}
            this->messageChannel = nullptr;
        }
        if (this->ssl != nullptr) {
            SSL_free(this->ssl);
            this->ssl = nullptr;
        }
    }

    void BinanceWsClient::release_resource() {

        this->stop();

        // std::unique_lock<std::shared_mutex> lock(this->rw_lock);

        // if (this->messageChannel != nullptr) {
        //     // clear channel data
        //     string gc;
        //     while (this->messageChannel->try_dequeue(gc)) {}
        // }

        // this->recvBuffer.clear();
        // this->msgBuffer.clear();
        // if (this->socketFd > 0) {
        //     try {
        //         close(this->socketFd);
        //     } catch (std::exception &e) {}
        // }
        // if (this->ssl != nullptr && SSL_get_shutdown(this->ssl) != SSL_SENT_SHUTDOWN) {
        //     try {
        //         SSL_shutdown(this->ssl);
        //         // SSL_free(this->ssl);
        //         // this->ssl = nullptr;
        //     } catch (std::exception &e) {}
        // }
        // this->isConnected = false;
        // this->isLogoned = false;
    }
}