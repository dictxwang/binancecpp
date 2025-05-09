#include "binancecpp/binance_futures.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceFuturesRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal);
    }

    void BinanceFuturesRestClient::get_exchangeInfo(CommonRestResponse<std::vector<binance::FuturesExchangeInfo>> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/exchangeInfo";

        binance::CommonRestResponse<std::string> action_response;
        std::vector<std::string> query_params;

        api_action_get_no_sec(url, query_params, action_response);

        if (action_response.code != 0) {
            response.code = action_response.code;
            response.msg = action_response.msg;
            return;
        }

        // Parse json value
        if (action_response.data.size() > 0) {
            Json::Value json_result;
            Json::Reader reader;
            json_result.clear();
            reader.parse(action_response.data, json_result);

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
            }
        }
    }
}