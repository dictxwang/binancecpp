#include "binancecpp/binance_ws.h"

namespace binance {

    void BinanceWsClient::setLocalIP(const std::string& localIP) {
        this->localIP = localIP;
    }
    void BinanceWsClient::setRemoteIP(const std::string& remoteIP) {
        this->remoteIP = remoteIP;
    }
    void BinanceWsClient::init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal, bool useCombine, bool isTrading) {
        this->apiKey = apiKey;
        this->secretKey = secretKey;
        this->marketType = marketType;
        this->useInternal = useInternal;
        if (isTrading) {
            this->wsEndpoint = getWsTradingEndpoint(marketType, useInternal);
        } else {
            if (useCombine) {
                this->wsEndpoint = getWsCombineBaseEndpoint(marketType, useInternal);
            } else {
                this->wsEndpoint = getWsBaseEndpoint(marketType, useInternal);
            }
        }
        this->localIP = "";
        this->remoteIP = "";
    }

    bool BinanceWsClient::connect_endpoint(std::string& handshakePath) {
        // Implementation of connecting to the WebSocket endpoint
        // This is where you would set up the WebSocket connection and handle messages

        // Step 1: Create a secure TLS connection
        int socket_fd = 0;
        SSL* ssl = nullptr;
        try {
            ssl = create_tls_connection(socket_fd, this->wsEndpoint.first, this->wsEndpoint.second, this->remoteIP, this->localIP);
        } catch (std::exception &e) {
            throw e;
        }

        this->socketFd = socket_fd;
        this->ssl = ssl;

        // Step 2: Perform the WebSocket handshake
        try {
            perform_websocket_handshake(ssl, this->wsEndpoint.first, handshakePath);
        } catch (std::exception &e) {
            throw e;
        }

        return true;
    }

    bool BinanceWsClient::send_subscribe(std::string& subscribeFrame) {

        if (subscribeFrame.size() == 0) {
            throw std::runtime_error("subscribe framme is empty");
        }

        int writeLength = SSL_write(this->ssl, subscribeFrame.c_str(), subscribeFrame.size());
        if (writeLength <= 0) {
            throw std::runtime_error("failed to send subscribe frame");
        }

        char buffer[1024];
        int len = SSL_read(this->ssl, buffer, sizeof(buffer) - 1);
        if (len < 0) {
            throw std::runtime_error("failed to receive subscribe response");
        } else {
            return true;
        }
    }

    bool BinanceWsClient::start_event_loop(WS_CB customCallback) {

        this->customCallback = customCallback;

        while(true) {
            try {
                int total_recv_len = 0;
                int max_buffer_size = 1024;
                char buffer[max_buffer_size];
                int read_len = 0;
                int pendding_len = 0;
                int recv_start_len = this->recvBuffer.length();
                do {
                    read_len = SSL_read(ssl, buffer, sizeof(buffer) - 1);
                    if (read_len > 0) {
                        this->recvBuffer.append(buffer, read_len);
                        total_recv_len += read_len;
                    } else {
                        break;
                    }
                    pendding_len = SSL_pending(ssl);
                } while (pendding_len > 0);

                if (read_len < 0) {
                    int err = SSL_get_error(ssl, read_len);
                    ERR_clear_error();
                    if (err == SSL_ERROR_WANT_READ) {
                        // No data available, continue reading
                        continue;
                    } else if (err == SSL_ERROR_WANT_WRITE) {
                        // No data available, continue reading
                        continue;
                    } else {
                        break;
                    }
                }

                if (total_recv_len <= 0) {
                    return false;
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
                // TODO print exception
                break;
            }
        }

        this->release_resource();

        return false;
    }

    bool BinanceWsClient::process_one_message(WebSocketPacket& packet, ByteBuffer& mssageBuffer) {
        
        if (packet.get_opcode() == WebSocketPacket::WSOpcode_Ping) {
            // Handle Ping
            // Send Pong response
            if (ssl == nullptr) {
                throw std::runtime_error("ssl is null while send pong package");
            }
            packet.set_fin(1);
            packet.set_opcode(WebSocketPacket::WSOpcode_Pong);
            ByteBuffer pong_buf;
            packet.pack_dataframe(pong_buf);
            int writeLength = SSL_write(ssl, pong_buf.bytes(), pong_buf.length());
            if (writeLength <= 0) {
                throw std::runtime_error("failed to write pong package");
            }
        } else if (packet.get_opcode() == WebSocketPacket::WSOpcode_Continue) {
            // Handle continuation frame
        }
        if (packet.get_opcode() == WebSocketPacket::WSOpcode_Text) {
            // Parse to json result
            Json::Value json_result;
            Json::Reader reader;
            json_result.clear();
            reader.parse(std::string(mssageBuffer.bytes(), mssageBuffer.length()).c_str() , json_result);

            // Process json result by custom
            return this->customCallback(json_result);
        }

        return true;
    }

    void BinanceWsClient::release_resource() {
        this->recvBuffer.clear();
        this->msgBuffer.clear();
        if (this->ssl != nullptr) {
            try {
                SSL_free(this->ssl);
            } catch (std::exception &e) {}
        }
        if (this->socketFd > 0) {
            try {
                close(this->socketFd);
            } catch (std::exception &e) {}
        }
    }
}
