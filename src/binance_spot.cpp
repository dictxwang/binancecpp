#include "binancecpp/binance_spot.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceSpotRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::SPOT, useInternal);
    }

    void BinanceSpotRestClient::get_exchangeInfo(std::vector<std::string>& instIds, CommonRestResponse<std::vector<binance::SpotExchangeInfo>> &response) {
        Json::Reader reader;
        std::string url = baseUrl + "/api/v3/exchangeInfo";
        if (instIds.size() > 0) {
            url.append("?");
            if (instIds.size() == 1) {
                url.append("symbol=").append(instIds.front());
            } else {
                for (int i = 0; i < instIds.size(); i++) {
                    instIds[i] = "\"" + instIds[i] + "\"";
                }
                url.append("symbols=[").append(strHelper::joinStrings(instIds, ",")).append("]");
            }
        }
        // TODO Initialize libcurl
        // curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();
    
        CURLcode res;
        std::string str_result;
        res = curl_api(curl, url, str_result);

        if (str_result.size() > 0 ) {
            try {
                Json::Value json_result;
                Json::Reader reader;
                json_result.clear();
                reader.parse(str_result , json_result);
                
                // parse json to model
                if (json_result.isMember("symbols")) {
                    Json::Value symbols = json_result["symbols"];
                    for (int i = 0; i < symbols.size(); i++) {
                        SpotExchangeInfo exchangeInfo;
                        exchangeInfo.symbol = symbols[i]["symbol"].asString();
                        exchangeInfo.status = symbols[i]["status"].asString();
                        exchangeInfo.baseAsset = symbols[i]["baseAsset"].asString();
                        exchangeInfo.quoteAsset = symbols[i]["quoteAsset"].asString();
                        if (symbols[i].isMember("filters")) {
                            for (int j = 0; j < symbols[i]["filters"].size(); j++) {
                                if (symbols[i]["filters"][j].isMember("filterType")) {
                                    if (symbols[i]["filters"][j]["filterType"].asString() == "PRICE_FILTER") {
                                        exchangeInfo.minPrice = str_to_dobule(symbols[i]["filters"][j]["minPrice"]);
                                        exchangeInfo.maxPrice = str_to_dobule(symbols[i]["filters"][j]["maxPrice"]);
                                        exchangeInfo.tickSize = str_to_dobule(symbols[i]["filters"][j]["tickSize"]);
                                    } else if (symbols[i]["filters"][j]["filterType"].asString() == "LOT_SIZE") {
                                        exchangeInfo.minQty = str_to_dobule(symbols[i]["filters"][j]["minQty"]);
                                        exchangeInfo.maxQty = str_to_dobule(symbols[i]["filters"][j]["maxQty"]);
                                        exchangeInfo.stepSize = str_to_dobule(symbols[i]["filters"][j]["stepSize"]);
                                    }
                                }
                            }
                        }
                        response.data.push_back(exchangeInfo);
                    }
                    response.code = 0;
                }
            } catch (std::exception &e ) {
                response.code = -900;
                response.msg = "parse json error: " + std::string(e.what());
                throw e;
            }
        } else {
            // notthing to parse
            response.code = -100;
            response.msg = "no response content";
        }

        if (curl != nullptr) {
            curl_easy_cleanup(curl);
        }
    }
} // namespace binance
