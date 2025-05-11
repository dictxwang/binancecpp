#include "binancecpp/binance_futures.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceFuturesRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::FUTURES, useInternal);
    }

    void BinanceFuturesRestClient::setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response) {

        std::string url = this->serverMeta.baseUrl + "/fapi/v1/time";
        std::vector<std::string> query_params;
        binance::CommonRestResponse<std::string> action_response;

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
            if (json_result.isMember("serverTime")) {
                uint64_t serverTime = json_result["serverTime"].asUInt64();
                response.data = serverTime;
                uint64_t now = get_current_ms_epoch();
                this->timeOffset = static_cast<long long>(now)-static_cast<long long>(serverTime);
            }
        }
    }

    uint64_t BinanceFuturesRestClient::get_property_timestamp() {
        
        return get_current_ms_epoch() - this->timeOffset;
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

    void BinanceFuturesRestClient::get_account_v2(CommonRestResponse<binance::FuturesAccount> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v2/account";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
        if (action_response.code != 0) {
            response.code = action_response.code;
            response.msg = action_response.msg;
            return;
        }

        // std::cout << "response data: " << action_response.data << std::endl;

        // Parse json value
        if (action_response.data.size() > 0) {
            Json::Value json_result;
            Json::Reader reader;
            json_result.clear();
            reader.parse(action_response.data, json_result);
            
            FuturesAccount account;
            account.feeTier = json_result["feeTier"].asInt();
            account.feeBurn = json_result["feeBurn"].asBool();
            account.canDeposit = json_result["canDeposit"].asBool();
            account.canWithdraw = json_result["canWithdraw"].asBool();
            if (json_result.isMember("updateTime")) {
                account.updateTime = json_result["updateTime"].asUInt64();
            }
            account.multiAssetsMargin = json_result["multiAssetsMargin"].asBool();
            account.tradeGroupId = json_result["tradeGroupId"].asInt();
            account.totalInitialMargin = str_to_dobule(json_result["totalInitialMargin"]);
            account.totalMaintMargin = str_to_dobule(json_result["totalMaintMargin"]);
            account.totalWalletBalance = str_to_dobule(json_result["totalWalletBalance"]);
            account.totalUnrealizedProfit = str_to_dobule(json_result["totalUnrealizedProfit"]);
            account.totalMarginBalance = str_to_dobule(json_result["totalMarginBalance"]);
            account.totalPositionInitialMargin = str_to_dobule(json_result["totalPositionInitialMargin"]);
            account.totalOpenOrderInitialMargin = str_to_dobule(json_result["totalOpenOrderInitialMargin"]);
            account.totalCrossWalletBalance = str_to_dobule(json_result["totalCrossWalletBalance"]);
            account.totalCrossUnPnl = str_to_dobule(json_result["totalCrossUnPnl"]);
            account.availableBalance = str_to_dobule(json_result["availableBalance"]);
            account.maxWithdrawAmount = str_to_dobule(json_result["maxWithdrawAmount"]);
            if (json_result.isMember("assets")) {
                Json::Value assets = json_result["assets"];
                for (int i = 0; i < assets.size(); i++) {
                    Json::Value asset = assets[i];
                    FuturesAccountAsset accountAsset;
                    accountAsset.asset = asset["asset"].asString();
                    accountAsset.walletBalance = str_to_dobule(asset["walletBalance"]);
                    accountAsset.unrealizedProfit = str_to_dobule(asset["unrealizedProfit"]);
                    accountAsset.marginBalance = str_to_dobule(asset["marginBalance"]);
                    accountAsset.maintMargin = str_to_dobule(asset["maintMargin"]);
                    accountAsset.initialMargin = str_to_dobule(asset["initialMargin"]);
                    accountAsset.positionInitialMargin = str_to_dobule(asset["positionInitialMargin"]);
                    accountAsset.openOrderInitialMargin = str_to_dobule(asset["openOrderInitialMargin"]);
                    accountAsset.crossWalletBalance = str_to_dobule(asset["crossWalletBalance"]);
                    accountAsset.crossUnPnl = str_to_dobule(asset["crossUnPnl"]);
                    accountAsset.availableBalance = str_to_dobule(asset["availableBalance"]);
                    accountAsset.maxWithdrawAmount = str_to_dobule(asset["maxWithdrawAmount"]);
                    accountAsset.marginAvailable = asset["marginAvailable"].asBool();
                    accountAsset.updateTime = asset["updateTime"].asUInt64();
                    account.assets.push_back(accountAsset);
                }
            }

            if (json_result.isMember("positions")) {
                Json::Value positions = json_result["positions"];
                for (int i = 0; i < positions.size(); i++) {
                    Json::Value position = positions[i];
                    FuturesAccountPosition accountPosition;
                    accountPosition.symbol = position["symbol"].asString();
                    accountPosition.initialMargin = str_to_dobule(position["initialMargin"]);
                    accountPosition.maintMargin = str_to_dobule(position["maintMargin"]);
                    accountPosition.unrealizedProfit = str_to_dobule(position["unrealizedProfit"]);
                    accountPosition.positionInitialMargin = str_to_dobule(position["positionInitialMargin"]);
                    accountPosition.openOrderInitialMargin = str_to_dobule(position["openOrderInitialMargin"]);
                    if (position["leverage"].isString()) {
                        accountPosition.leverage = std::stoi(position["leverage"].asString());
                    } else if (position["leverage"].isInt()) {
                        accountPosition.leverage = position["leverage"].asInt();
                    }
                    accountPosition.isolated = position["isolated"].asBool();
                    accountPosition.entryPrice = str_to_dobule(position["entryPrice"]);
                    accountPosition.maxNotional = str_to_dobule(position["maxNotional"]);
                    accountPosition.bidNotional = str_to_dobule(position["bidNotional"]);
                    accountPosition.askNotional = str_to_dobule(position["askNotional"]);
                    accountPosition.positionSide = position["positionSide"].asString();
                    accountPosition.positionAmt = str_to_dobule(position["positionAmt"]);
                    accountPosition.updateTime = position["updateTime"].asUInt64();
                    account.positions.push_back(accountPosition);
                }
            }
            response.data = account;
        }
    }
    
    void BinanceFuturesRestClient::get_commissionRate(std::string &symbol, CommonRestResponse<binance::FuturesCommissionRate> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/commissionRate";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("symbol=" + symbol);
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            FuturesCommissionRate rate;
            rate.symbol = json_result["symbol"].asString();
            rate.makerCommissionRate = str_to_dobule(json_result["makerCommissionRate"]);
            rate.takerCommissionRate = str_to_dobule(json_result["takerCommissionRate"]);
            response.data = rate;
        }
    }
    
    void BinanceFuturesRestClient::get_accountConfig(CommonRestResponse<binance::FuturesAccountConfig> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/accountConfig";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            FuturesAccountConfig config;
            config.feeTier = json_result["feeTier"].asInt();
            config.canTrade = json_result["canTrade"].asBool();
            config.canDeposit = json_result["canDeposit"].asBool();
            config.canWithdraw = json_result["canWithdraw"].asBool();
            config.dualSidePosition = json_result["dualSidePosition"].asBool();
            if (json_result.isMember("updateTime")) {
                config.updateTime = json_result["updateTime"].asUInt64();
            }
            config.multiAssetsMargin = json_result["multiAssetsMargin"].asBool();
            config.tradeGroupId = json_result["tradeGroupId"].asInt();

            response.data = config;
        }
    }
    
    void BinanceFuturesRestClient::get_symbolConfig(std::string &symbol, CommonRestResponse<binance::FuturesSymbolConfig> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/symbolConfig";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        if (symbol.size() > 0) {
            query_params.push_back("symbol=" + symbol);
        }
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            FuturesSymbolConfig config;
            config.symbol = json_result["symbol"].asString();
            config.marginType = json_result["marginType"].asString();
            config.isAutoAddMargin = json_result["isAutoAddMargin"].asBool();
            if (json_result["leverage"].isString()) {
                config.leverage = std::stoi(json_result["leverage"].asString());
            } else if (json_result["leverage"].isInt()) {
                config.leverage = json_result["leverage"].asInt();
            }
            config.maxNotionalValue = str_to_dobule(json_result["maxNotionalValue"]);

            response.data = config;
        }
    }

    void BinanceFuturesRestClient::get_orderRateLimit(CommonRestResponse<std::vector<binance::OrderRateLimit>> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/rateLimit/order";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            if (json_result.isArray()) {
                std::vector<binance::OrderRateLimit> data;
                for (int i = 1; i < json_result.size(); i++) {
                    binance::OrderRateLimit rateLimit;
                    rateLimit.rateLimitType = json_result[i]["rateLimitType"].asString();
                    rateLimit.interval = json_result[i]["interval"].asString();
                    rateLimit.intervalNum = json_result[i]["intervalNum"].asInt();
                    rateLimit.limit = json_result[i]["limit"].asInt();
                    data.push_back(rateLimit);
                }
                response.data = data;
            }
        }
    }

    void BinanceFuturesRestClient::get_multiAssetMargin(CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/multiAssetsMargin";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            if (json_result.isMember("multiAssetsMargin")) {
                response.data = json_result["multiAssetsMargin"].asBool();
            } else {
                response.code = -404;
                response.msg = "multiAssetsMargin not found";
            }
        }
    }

    void BinanceFuturesRestClient::get_positionSideDual(CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/positionSide/dual";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            if (json_result.isMember("dualSidePosition")) {
                response.data = json_result["dualSidePosition"].asBool();
            } else {
                response.code = -404;
                response.msg = "dualSidePosition not found";
            }
        }
    }

    void BinanceFuturesRestClient::get_bnbFeeBurn(CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/feeBurn";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "GET";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, empty, action_response);
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

            if (json_result.isMember("feeBurn")) {
                response.data = json_result["feeBurn"].asBool();
            } else {
                response.code = -404;
                response.msg = "feeBurn not found";
            }
        }
    }
    void BinanceFuturesRestClient::toggle_bnbFeeBurn(bool feeBurn, CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/feeBurn";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("feeBurn=" + strHelper::toString(feeBurn));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, body_params, action_response);
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

            if (json_result.isMember("code")) {
                int code = json_result["code"].asInt();
                if (code == 200) {
                    response.data = feeBurn;
                } else {
                    response.code = code;
                    if (json_result.isMember("msg")) {
                        response.msg = json_result["msg"].asString();
                    }
                }
            } else {
                response.code = -404;
                response.msg = "result not return";
            }
        }
    }
    void BinanceFuturesRestClient::change_marginType(std::string &symbol, std::string &marginType, CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/marginType";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("symbol=" + symbol);
        body_params.push_back("marginType=" + marginType);
        api_action(url, binance::SecTypeSignature, action, empty, query_params, body_params, action_response);
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

            if (json_result.isMember("code")) {
                int code = json_result["code"].asInt();
                if (code == 200) {
                    response.data = true;
                } else {
                    response.code = code;
                    if (json_result.isMember("msg")) {
                        response.msg = json_result["msg"].asString();
                    }
                }
            } else {
                response.code = -404;
                response.msg = "result not return";
            }
        }
    }

    void BinanceFuturesRestClient::change_positionSideDual(bool dualSidePosition, CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/positionSide/dual";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("dualSidePosition=" + strHelper::toString(dualSidePosition));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, body_params, action_response);
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

            if (json_result.isMember("code")) {
                int code = json_result["code"].asInt();
                if (code == 200) {
                    response.data = true;
                } else {
                    response.code = code;
                    if (json_result.isMember("msg")) {
                        response.msg = json_result["msg"].asString();
                    }
                }
            } else {
                response.code = -404;
                response.msg = "result not return";
            }
        }
    }

    void BinanceFuturesRestClient::change_initialLeverage(std::string &symbol, int leverage, CommonRestResponse<FuturesChangeLeverageResult> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/leverage";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("symbol=" + symbol);
        body_params.push_back("leverage=" + std::to_string(leverage));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, body_params, action_response);
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

            binance::FuturesChangeLeverageResult result;
            if (json_result.isMember("symbol")) {
                result.symbol = json_result["symbol"].asString();
            }
            if (json_result.isMember("leverage")) {
                result.leverage = json_result["leverage"].asInt();
            }
            if (json_result.isMember("maxNotionalValue")) {
                result.maxNotionalValue = str_to_dobule(json_result["maxNotionalValue"]);
            }

            response.data = result;
        }
    }

    void BinanceFuturesRestClient::change_multiAssetsMargin(bool multiAssetsMargin, CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/multiAssetsMargin";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("multiAssetsMargin=" + strHelper::toString(multiAssetsMargin));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, body_params, action_response);
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

            if (json_result.isMember("code")) {
                int code = json_result["code"].asInt();
                if (code == 200) {
                    response.data = true;
                } else {
                    response.code = code;
                    if (json_result.isMember("msg")) {
                        response.msg = json_result["msg"].asString();
                    }
                }
            } else {
                response.code = -404;
                response.msg = "result not return";
            }
        }
    }

    void BinanceFuturesRestClient::modify_isolatedPositionMargin(std::string &symbol, std::string &positionSide, double amount, int type, CommonRestResponse<bool> &response) {
        std::string url = this->serverMeta.baseUrl + "/fapi/v1/positionMargin";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("symbol=" + symbol);
        if (positionSide.size() > 0) {
            body_params.push_back("positionSide=" + positionSide);
        }
        body_params.push_back("amount=" + strHelper::toString(amount));
        body_params.push_back("type=" + strHelper::toString(type));
        api_action(url, binance::SecTypeSignature, action, empty, query_params, body_params, action_response);
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

            if (json_result.isMember("code")) {
                int code = json_result["code"].asInt();
                if (code == 200) {
                    response.data = true;
                } else {
                    response.code = code;
                    if (json_result.isMember("msg")) {
                        response.msg = json_result["msg"].asString();
                    }
                }
            } else {
                response.code = -404;
                response.msg = "result not return";
            }
        }
    }

}