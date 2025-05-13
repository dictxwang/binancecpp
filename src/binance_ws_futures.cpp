#include "binancecpp/binance_ws_futures.h"

namespace binance {

    void BinanceFuturesWsClient::initBookTickerV1(bool useInternal, bool useCombine) {
        BinanceWsClient::init("","", MarketType::FUTURES, useInternal, useCombine, false);
    }

    bool BinanceFuturesWsClient::startBookTickerV1(WS_CB customCallback, std::vector<std::string>& symbols) {
        
        this->release_resource();

        if (symbols.size() == 0) {
            return false;
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

        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop(customCallback);
    }

    bool BinanceFuturesWsClient::startAllBookTickersV1(WS_CB customCallback) {
                
        this->release_resource();

        std::string endpoint = this->wsEndpoint.third;
        
        if (!this->useCombine && !this->useTrading) {
            endpoint.append("/");
        }
        endpoint.append("!bookTicker");
        
        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop(customCallback);
    }

    void BinanceFuturesWsClient::initMarkPriceV1(bool useInternal, bool useCombine) {
        BinanceWsClient::init("","", MarketType::FUTURES, useInternal, useCombine, false);
    }

    bool BinanceFuturesWsClient::startMarkPriceV1(WS_CB customCallback, std::vector<std::string>& symbols, binance::WsMarkPriceInterval interval) {
                
        this->release_resource();

        if (symbols.size() == 0) {
            return false;
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

        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop(customCallback);
    }

    bool BinanceFuturesWsClient::startAllMarkPricesV1(WS_CB customCallback, binance::WsMarkPriceInterval interval) {
                
        this->release_resource();

        std::string endpoint = this->wsEndpoint.third;

        if (!this->useCombine && !this->useTrading) {
            endpoint.append("/");
        }
        endpoint.append("!markPrice@arr");
        if (interval == binance::WsMPOneSecond) {
            endpoint.append("@1s");
        }
        
        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop(customCallback);
    }

    void BinanceFuturesWsClient::initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal) {
        BinanceWsClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal, false, false);
    }

    bool BinanceFuturesWsClient::startUserDataStreamV1(WS_CB customCallback, std::string listenKey) {
        
        this->release_resource();

        if (listenKey.size() == 0) {
            return false;
        }

        std::string endpoint = this->wsEndpoint.third + "/" + listenKey;
        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // Not require logon and subscribe

        return this->start_event_loop(customCallback);
    }

    void BinanceFuturesWsClient::initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal) {

        BinanceWsClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal, false, true);
    }

    bool BinanceFuturesWsClient::startUserDataStream(WS_CB customCallback) {
        
        this->release_resource();

        std::string endpoint = this->wsEndpoint.third;
        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // session logon
        result = this->send_session_logon();
        if (!result) {
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
        if (!result) {
            return result;
        }

        return this->start_event_loop(customCallback);
    }

    void BinanceFuturesWsClient::initOrderService(std::string apiKey, std::string secretKey, bool useInternal) {
        BinanceWsClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal, false, true);
    }

    bool BinanceFuturesWsClient::startOrderService(WS_CB customCallback) {

        release_resource();

        std::string endpoint = this->wsEndpoint.third;
        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // session logon
        result = this->send_session_logon();
        if (!result) {
            return result;
        }
        return this->start_event_loop(customCallback);
    }

    bool BinanceFuturesWsClient::placeOrder(binance::FuturesNewOrder &order) {
        
        // TODO  use lock
        this->lock.lock();
        this->lock.unlock();

    }

    bool BinanceFuturesWsClient::cancelOrder(binance::FuturesCancelOrder &order) {

        // TODO  use lock
        this->lock.lock();
        this->lock.unlock();
    }
}