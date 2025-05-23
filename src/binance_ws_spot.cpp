#include "binancecpp/binance_ws_spot.h"

namespace binance {

    void BinanceSpotWsClient::initBookTicker(bool useInternal, bool useCombine) {

        BinanceWsClient::init("","", MarketType::SPOT, useInternal, useCombine, false);
    }

    std::pair<bool, string> BinanceSpotWsClient::startBookTicker(std::vector<std::string>& symbols) {
        
        // release the connection if existed, aviod exception by multi call
        // this->release_resource();

        if (symbols.size() == 0) {
            return std::pair<bool, string>(false, "symbols is empty");
        }

        std::pair<bool, string> result = this->connect_endpoint(this->wsEndpoint.third);
        if (!result.first) {
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
        if (!result.first) {
            return result;
        }

        return this->start_event_loop();
    }

    void BinanceSpotWsClient::initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal) {

        BinanceWsClient::init(apiKey, secretKey, MarketType::SPOT, useInternal, false, true);
    }

    void BinanceSpotWsClient::initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal) {

        BinanceWsClient::init(apiKey, secretKey, MarketType::SPOT, useInternal, false, false);
    }

    std::pair<bool, string> BinanceSpotWsClient::startUserDataStreamV1(std::string listenKey) {
        
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

    std::pair<bool, string> BinanceSpotWsClient::startUserDataStream() {
        
        // this->release_resource();

        std::cout << "A" << std::endl;
        std::string endpoint = this->wsEndpoint.third;
        std::pair<bool, string> result = this->connect_endpoint(endpoint);
        if (!result.first) {
            return result;
        }

        std::cout << "B" << std::endl;
        // session logon
        result = this->send_session_logon();
        if (!result.first) {
            return result;
        }

        std::cout << "C" << std::endl;
        // subscribe
        Json::Value reqJson;
        reqJson["id"] = this->sessionID;
        reqJson["method"] = "userDataStream.subscribe";
        std::string subscribeMessage = serialize_json_value(reqJson);

        std::cout << "D" << std::endl;
        result = this->send_subscribe(subscribeMessage);
        if (!result.first) {
            return result;
        }

        std::cout << "E" << std::endl;
        return this->start_event_loop();
    }
}