#include "binancecpp/binance_ws_futures.h"

namespace binance {

    void BinanceFuturesWsClient::initBookTickerV1(bool useInternal, bool useCombine) {
        BinanceWsClient::init("","", MarketType::FUTURES, useInternal, useCombine, false);
    }

    bool BinanceFuturesWsClient::startBookTickerV1(WS_CB customCallback, std::vector<std::string>& symbols) {
        
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
}