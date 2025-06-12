#include "binancecpp/binance_ws_futures.h"

namespace binance {

    void BinanceFuturesWsClient::initBookTickerV1(bool useInternal, bool useCombine) {
        BinanceWsClient::init("","", MarketType::FUTURES, useInternal, useCombine, false);
    }

    std::pair<bool, string> BinanceFuturesWsClient::startBookTickerV1(std::vector<std::string> symbols) {

        if (symbols.size() == 0) {
            return std::pair<bool, string>(false, "subscribe symbols is empty");
        }

        std::string listenPath;
        for (size_t i = 0; i < symbols.size(); ++i) {
            listenPath.append(strHelper::toLower(symbols[i])).append("@bookTicker");
            if (i < symbols.size() - 1) {
                listenPath.append("/");
            }
        }
        std::string endpoint = this->wsEndpoint.third;
        if (!this->useCombine && !this->useTrading) {
            endpoint.append("/");
        }
        endpoint.append(listenPath);

        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }    

        // Not require logon and subscribe

        return this->start_event_loop();
    }

    std::pair<bool, string> BinanceFuturesWsClient::startAllBookTickersV1() {
                
        // this->release_resource();

        std::string endpoint = this->wsEndpoint.third;
        
        if (!this->useCombine && !this->useTrading) {
            endpoint.append("/");
        }
        endpoint.append("!bookTicker");
        
        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop();
    }

    void BinanceFuturesWsClient::initMarkPriceV1(bool useInternal, bool useCombine) {
        BinanceWsClient::init("","", MarketType::FUTURES, useInternal, useCombine, false);
    }

    std::pair<bool, string> BinanceFuturesWsClient::startMarkPriceV1(std::vector<std::string> symbols, binance::WsMarkPriceInterval interval) {
                
        // this->release_resource();

        if (symbols.size() == 0) {
            return std::pair<bool, string>(false, "subscribe symbols is empty");
        }

        std::string listenPath;
        for (size_t i = 0; i < symbols.size(); ++i) {
            listenPath.append(strHelper::toLower(symbols[i])).append("@markPrice");
            if (interval == binance::WsMPOneSecond) {
                listenPath.append("@1s");
            }
            if (i < symbols.size() - 1) {
                listenPath.append("/");
            }
        }
        std::string endpoint = this->wsEndpoint.third;
        if (!this->useCombine && !this->useTrading) {
            endpoint.append("/");
        }
        endpoint.append(listenPath);

        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop();
    }

    std::pair<bool, string> BinanceFuturesWsClient::startAllMarkPricesV1(binance::WsMarkPriceInterval interval) {
                
        // this->release_resource();

        std::string endpoint = this->wsEndpoint.third;

        if (!this->useCombine && !this->useTrading) {
            endpoint.append("/");
        }
        endpoint.append("!markPrice@arr");
        if (interval == binance::WsMPOneSecond) {
            endpoint.append("@1s");
        }
        
        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop();
    }

    void BinanceFuturesWsClient::initUserDataStreamV1(const std::string apiKey, const std::string secretKey, bool useInternal) {
        BinanceWsClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal, false, false);
    }

    std::pair<bool, string> BinanceFuturesWsClient::startUserDataStreamV1(const std::string listenKey) {
        
        // this->release_resource();

        if (listenKey.size() == 0) {
            return std::pair<bool, string>(false, "require listenKey");
        }

        std::string endpoint = this->wsEndpoint.third + "/" + listenKey;
        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop();
    }

    void BinanceFuturesWsClient::initUserDataStream(const std::string apiKey, const std::string secretKey, bool useInternal) {

        BinanceWsClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal, false, true);
    }

    std::pair<bool, string> BinanceFuturesWsClient::startUserDataStream() {
        
        // this->release_resource();

        std::string endpoint = this->wsEndpoint.third;
        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        // session logon
        result = this->send_session_logon();
        if (!result.first) {
            return result;
        }

        // start
        Json::Value paramsJson;
        paramsJson["apiKey"] = this->apiKey;
        Json::Value reqJson;
        reqJson["id"] = this->sessionID;
        reqJson["method"] = "userDataStream.start";
        reqJson["params"] = paramsJson;
        std::string subscribeMessage = serialize_json_value(reqJson);

        result = this->send_subscribe(subscribeMessage);
        if (!result.first) {
            return result;
        }

        return this->start_event_loop();
    }

    void BinanceFuturesWsClient::initOrderService(const std::string apiKey, const std::string secretKey, bool useInternal) {
        BinanceWsClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal, false, true);
    }

    std::pair<bool, string> BinanceFuturesWsClient::startOrderService() {

        // release_resource();

        std::string endpoint = this->wsEndpoint.third;
        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        // session logon
        result = this->send_session_logon();
        if (!result.first) {
            return result;
        }

        return this->start_event_loop();
    }

    std::pair<bool, string> BinanceFuturesWsClient::placeOrder(binance::FuturesNewOrder &order) {

        if (!this->isConnected || !this->isLogoned) {
            return std::pair<bool, string>(false, "connection closed or session expired");
        }

        uint64_t timestamp = get_current_ms_epoch();
        std::string reqId = generate_uuid();

        Json::Value paramsJson;
        // Warning: If give apiKey, signature will be also required.
        // paramsJson["apiKey"] = this->apiKey;
        paramsJson["timestamp"] = Json::UInt64(timestamp);

        paramsJson["symbol"] = order.symbol;
        paramsJson["side"] = order.side;
        paramsJson["positionSide"] = order.positionSide;
        paramsJson["type"] = order.type;
        paramsJson["quantity"] = order.quantity;
        paramsJson["timeInForce"] = order.timeInForce;
        if (order.price != 0) {
            paramsJson["price"] = order.price;
        }
        if (order.reduceOnly == "true" || order.reduceOnly == "false") {
            paramsJson["reduceOnly"] = order.reduceOnly;
        }
        if (order.newClientOrderId.size() > 0) {
            paramsJson["newClientOrderId"] = order.newClientOrderId;
        }
        if (order.newOrderRespType.size() > 0) {
            paramsJson["newOrderRespType"] = order.newOrderRespType;
        }

        Json::Value reqJson;
        reqJson["id"] = reqId;
        reqJson["method"] = "order.place";
        reqJson["params"] = paramsJson;
        std::string payload = serialize_json_value(reqJson);

        // std::cout << "place order payload: " << payload << std::endl;

        WebSocketPacket packet;
        packet.set_fin(1);
        packet.set_opcode(WebSocketPacket::WSOpcode_Text);
        packet.set_mask(1);
        ByteBuffer messageBuffer;
        packet.set_payload(payload.c_str(), payload.length());
        packet.pack_dataframe(messageBuffer);

        int writeLength = 0;
        string writeError;

        std::unique_lock<std::shared_mutex> lock(this->rw_lock);
        // this->lock.lock();
        try {
            writeLength = SSL_write(ssl, messageBuffer.bytes(),  messageBuffer.length());
        } catch (std::exception &e) {
            writeError = e.what();
        }
        // this->lock.unlock();

        if (writeLength <= 0) {
            return std::pair<bool, string>(false, "failed to send place order frame: " + writeError);
        }

        return std::pair<bool, string>(true, "");
    }

    std::pair<bool, string> BinanceFuturesWsClient::cancelOrder(binance::FuturesCancelOrder &order) {

        if (!this->isConnected || !this->isLogoned) {
            return std::pair<bool, string>(false, "connection closed or session expired");
        }
        
        uint64_t timestamp = get_current_ms_epoch();
        std::string reqId = generate_uuid();

        Json::Value paramsJson;
        // Warning: If give apiKey, signature will be also required.
        // paramsJson["apiKey"] = this->apiKey;
        paramsJson["timestamp"] = Json::UInt64(timestamp);

        paramsJson["symbol"] = order.symbol;
        if (order.origClientOrderId.size() > 0) {
            paramsJson["origClientOrderId"] = order.origClientOrderId;
        } else {
            paramsJson["orderId"] = Json::UInt64(order.orderId);
        }

        Json::Value reqJson;
        reqJson["id"] = reqId;
        reqJson["method"] = "order.cancel";
        reqJson["params"] = paramsJson;
        std::string payload = serialize_json_value(reqJson);

        // std::cout << "cancel order payload: " << payload << std::endl;

        WebSocketPacket packet;
        packet.set_fin(1);
        packet.set_opcode(WebSocketPacket::WSOpcode_Text);
        packet.set_mask(1);
        ByteBuffer messageBuffer;
        packet.set_payload(payload.c_str(), payload.length());
        packet.pack_dataframe(messageBuffer);

        int writeLength = 0;
        string writeError;

        std::unique_lock<std::shared_mutex> lock(this->rw_lock);
        // this->lock.lock();
        try {
            writeLength = SSL_write(ssl, messageBuffer.bytes(),  messageBuffer.length());
        } catch (std::exception &e) {
            writeError = e.what();
        }
        // this->lock.unlock();
        
        if (writeLength <= 0) {
            return std::pair<bool, string>(false, "failed to send cancel order frame: " + writeError);
        }

        return std::pair<bool, string>(true, "");
    }
}