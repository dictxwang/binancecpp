#include "binancecpp/binance_spot.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceSpotRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::SPOT, useInternal);
    }

    uint64_t BinanceSpotRestClient::get_property_timestamp() {
        
        return get_current_ms_epoch() - this->timeOffset;
    }

    void BinanceSpotRestClient::setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response) {

        std::string url = this->serverMeta.baseUrl + "/api/v3/time";
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

    void BinanceSpotRestClient::get_exchangeInfo(std::vector<std::string>& instIds, CommonRestResponse<std::vector<binance::SpotExchangeInfo>> &response) {
        std::string url = this->serverMeta.baseUrl + "/api/v3/exchangeInfo";
        binance::CommonRestResponse<std::string> action_response;

        std::vector<std::string> query_params;
        if (instIds.size() > 0) {
            if (instIds.size() == 1) {
                query_params.push_back("symbol=" + instIds.front());
            } else {
                for (size_t i = 0; i < instIds.size(); ++i) {
                    instIds[i] = "\"" + instIds[i] + "\"";
                }
                query_params.push_back("symbols=[" + strHelper::joinStrings(instIds, ",") + "]");
            }
        }

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

            if (json_result.isMember("symbols")) {
                Json::Value symbols = json_result["symbols"];
                for (int i = 0; i < symbols.size(); i++) {
                    SpotExchangeInfo exchangeInfo;
                    exchangeInfo.symbol = symbols[i]["symbol"].asString();
                    exchangeInfo.status = symbols[i]["status"].asString();
                    exchangeInfo.baseAsset = symbols[i]["baseAsset"].asString();
                    exchangeInfo.quoteAsset = symbols[i]["quoteAsset"].asString();
                    if (symbols[i].isMember("orderTypes")) {
                        for (int j = 0; j < symbols[i]["orderTypes"].size(); j++) {
                            exchangeInfo.orderTypes.push_back(symbols[i]["orderTypes"][j].asString());
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

        }
    }

    void BinanceSpotRestClient::get_account(CommonRestResponse<binance::SpotAccount> &response) {
        std::string url = this->serverMeta.baseUrl + "/api/v3/account";

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
    
            SpotAccount account;
            account.makerCommission = json_result["makerCommission"].asInt();
            account.takerCommission = json_result["takerCommission"].asInt();
            account.buyerCommission = json_result["buyerCommission"].asInt();
            account.sellerCommission = json_result["sellerCommission"].asInt();
            if (json_result.isMember("commissionRates")) {
                CommissionRates commissionRates;
                commissionRates.maker = str_to_dobule(json_result["commissionRates"]["maker"]);
                commissionRates.taker = str_to_dobule(json_result["commissionRates"]["taker"]);
                commissionRates.buyer = str_to_dobule(json_result["commissionRates"]["buyer"]);
                commissionRates.seller = str_to_dobule(json_result["commissionRates"]["seller"]);
                account.commissionRates = commissionRates;
            }
            account.canTrade = json_result["canTrade"].asBool();
            account.canWithdraw = json_result["canWithdraw"].asBool();
            account.canDeposit = json_result["canDeposit"].asBool();
            account.brokered = json_result["brokered"].asBool();
            account.requireSelfTradePrevention = json_result["requireSelfTradePrevention"].asBool();
            account.preventSor = json_result["preventSor"].asBool();
            account.updateTime = json_result["updateTime"].asUInt64();
            account.accountType = json_result["accountType"].asString();
            if (json_result.isMember("balances")) {
                Json::Value balances = json_result["balances"];
                for (int i = 0; i < balances.size(); i++) {
                    BalanceLite lite;
                    lite.asset = balances[i]["asset"].asString();
                    lite.free = str_to_dobule(balances[i]["free"]);
                    lite.locked = str_to_dobule(balances[i]["locked"]);
                    account.balances.push_back(lite);
                }
            }
            if (json_result.isMember("permissions")) {
                Json::Value permissions = json_result["permissions"];
                for (int i = 0; i < permissions.size(); i++) {
                    account.permissions.push_back(permissions[i].asString());
                }
            }
            account.uid = json_result["uid"].asUInt64();

            response.data = account;
        }
    }

    void BinanceSpotRestClient::start_userDataStream(CommonRestResponse<std::string> &response) {
        std::string url = this->serverMeta.baseUrl + "/api/v3/userDataStream";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        api_action(url, binance::SecTypeApiKey, action, empty, empty, empty, action_response);

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

            if (json_result.isMember("listenKey")) {
                response.data = json_result["listenKey"].asString();
            }
        }
    }

    void BinanceSpotRestClient::keep_userDataStream(const std::string listenKey, CommonRestResponse<std::string> &response) {
        
        std::string url = this->serverMeta.baseUrl + "/api/v3/userDataStream";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "PUT";
        std::vector <std::string> form_params;
        std::vector <std::string> empty;
        form_params.push_back("listenKey=" + listenKey);
        api_action(url, binance::SecTypeApiKey, action, empty, empty, form_params, action_response);

        if (action_response.code != 0) {
            response.code = action_response.code;
            response.msg = action_response.msg;
            return;
        }
    }

    void BinanceSpotRestClient::create_new_order(SpotNewOrder& order, CommonRestResponse<SpotNewOrderResult> &response) {
        // TODO
    }

} // namespace binance
