#include "binancecpp/binance_futures.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceFuturesRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal);
    }

    void BinanceFuturesRestClient::get_exchangeInfo(CommonRestResponse<std::vector<binance::FuturesExchangeInfo>> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/exchangeInfo";

        // TODO Initialize libcurl
        // curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();
    
        CURLcode res;
        std::string str_result;

        try {
            res = curl_api(curl, url, str_result);
        } catch (std::exception &e) {
            response.code = -500;
            response.msg = "parse json error: " + std::string(e.what());
            if (curl != nullptr) {
                curl_easy_cleanup(curl);
            }
            return;
        }

        if (str_result.size() > 0 ) {
            try {
                Json::Value json_result;
                Json::Reader reader;
                json_result.clear();
                reader.parse(str_result , json_result);

                if (parse_api_has_error(json_result, response)) {
                    if (curl != nullptr) {
                        curl_easy_cleanup(curl);
                    }
                    return;
                }
                
                // parse json to model
                if (json_result.isMember("symbols")) {
                    Json::Value symbols = json_result["symbols"];
                    for (int i = 0; i < symbols.size(); i++) {
                        FuturesExchangeInfo exchangeInfo;
                        exchangeInfo.symbol = symbols[i]["symbol"].asString();
                        if (symbols[i].isMember("pair")) {
                            exchangeInfo.pair = symbols[i]["pair"].asString();
                        }
                        if (symbols[i].isMember("contractType")) {
                            exchangeInfo.contractType = symbols[i]["contractType"].asString();
                        }
                        exchangeInfo.status = symbols[i]["status"].asString();
                        exchangeInfo.baseAsset = symbols[i]["baseAsset"].asString();
                        exchangeInfo.quoteAsset = symbols[i]["quoteAsset"].asString();
                        if (symbols[i].isMember("orderType")) {
                            for (int j = 0; j < symbols[i]["orderType"].size(); j++) {
                                exchangeInfo.orderTypes.push_back(symbols[i]["orderType"][j].asString());
                            }
                        }
                        if (symbols[i].isMember("timeInForce")) {
                            for (int j = 0; j < symbols[i]["timeInForce"].size(); j++) {
                                exchangeInfo.timeInForces.push_back(symbols[i]["timeInForce"][j].asString());
                            }
                        }
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
}