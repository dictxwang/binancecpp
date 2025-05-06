#include "binancecpp/binance_ws_spot.h"

namespace binance {

    void BinanceSpotWsClient::initBookTicker(bool useInternal, bool useCombine) {

        BinanceWsClient::init("","", MarketType::SPOT, useInternal, useCombine, false);
    }

    bool BinanceSpotWsClient::startBookTicker(WS_CB customCallback, std::vector<std::string>& symbols) {

        if (symbols.size() == 0) {
            return false;
        }

        bool result = this->connect_endpoint(this->wsEndpoint.third);
        if (!result) {
            return result;
        }

        int index = 0;
        std::string subscribe_msg = "{\"method\":\"SUBSCRIBE\",\"params\":[";
        for (std::string symbol : symbols) {
            if (index > 0) {
                subscribe_msg += ",";
            }
			subscribe_msg += "\"" + strHelper::toLower(symbol) + "@bookTicker\"";
            index++;
        }
        subscribe_msg += "]}";

        result = this->send_subscribe(subscribe_msg);
        // Another message pakcage
        // subscribe_msg = "\x81" + std::string(1, subscribe_msg.size()) + subscribe_msg; // Simplified frame
        // result = this->send_subscribe(subscribe_msg);
        if (!result) {
            return result;
        }

        return this->start_event_loop(customCallback);
    }

    void BinanceSpotWsClient::initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal) {

        BinanceWsClient::init(apiKey, secretKey, MarketType::SPOT, useInternal, false, false);
    }

    bool BinanceSpotWsClient::startUserDataStream(WS_CB customCallback , std::string listenKey) {

        if (listenKey.size() == 0) {
            return false;
        }

        std::string endpoint = this->wsEndpoint.third + "/" + listenKey;
        bool result = this->connect_endpoint(endpoint);
        if (!result) {
            return result;
        }

        // int index = 0;
        // std::string subscribe_msg = "{\"method\":\"SUBSCRIBE\",\"params\":[";
        // for (std::string symbol : symbols) {
        //     if (index > 0) {
        //         subscribe_msg += ",";
        //     }
		// 	subscribe_msg += "\"" + strHelper::toLower(symbol) + "@bookTicker\"";
        //     index++;
        // }
        // subscribe_msg += "]}";

        // result = this->send_subscribe(subscribe_msg);
        // if (!result) {
        //     return result;
        // }

        return this->start_event_loop(customCallback);
    }
}