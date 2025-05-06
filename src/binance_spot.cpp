#include "binancecpp/binance_spot.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceSpotRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::SPOT, useInternal);
    }

    void BinanceSpotRestClient::get_exchangeInfo(std::vector<std::string>& instIds, CommonRestResponse<std::vector<binance::SpotExchangeInfo>> &response) {
        std::string url = baseUrl + "/api/v3/exchangeInfo";
        if (instIds.size() > 0) {
            url.append("?");
            if (instIds.size() == 1) {
                url.append("symbol=").append(instIds.front());
            } else {
                for (size_t i = 0; i < instIds.size(); ++i) {
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

    void BinanceSpotRestClient::get_account(CommonRestResponse<binance::SpotAccount> &response) {
        std::string url = this->baseUrl + "/api/v3/account?";

        std::string querystring("timestamp=");
        querystring.append(std::to_string( get_current_ms_epoch()));

        std::string signature = hmac_sha256(this->secretKey.c_str() , querystring.c_str());
        querystring.append("&signature=");
        querystring.append(signature);

        url.append( querystring );
        std::vector<std::string> extra_http_header;
        std::string header_chunk("X-MBX-APIKEY: ");
        header_chunk.append(this->apiKey);
        extra_http_header.push_back(header_chunk);

        std::string post_data = "";
        std::string action = "GET";
        
        CURL* curl = curl_easy_init();
        CURLcode res;
        std::string str_result;

        try {
            // std::cout << "request signature= " << signature << std::endl;
            res = curl_api_with_header(curl, url, str_result , extra_http_header, post_data, action);
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
                // std::cout << "curl response text: " << str_result << std::endl;
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
            } catch (std::exception &e) {
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

    void BinanceSpotRestClient::start_userDataStream(CommonRestResponse<std::string> &response) {
        std::string url = this->baseUrl + "/api/v3/userDataStream";
        
        std::vector<std::string> extra_http_header;
        std::string header_chunk("X-MBX-APIKEY: ");
        header_chunk.append(this->apiKey);
        extra_http_header.push_back(header_chunk);

        std::string post_data = "";
        std::string action = "POST";
        
        CURL* curl = curl_easy_init();
        CURLcode res;
        std::string str_result;

        try {
            res = curl_api_with_header(curl, url, str_result , extra_http_header, post_data, action);
        } catch (std::exception &e) {
            response.code = -500;
            response.msg = "parse json error: " + std::string(e.what());
            if (curl != nullptr) {
                curl_easy_cleanup(curl);
            }
            return;
        }

        if (str_result.size() > 0 ) {
            // std::cout << "curl response text: " << str_result << std::endl;
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

            if (json_result.isMember("listenKey")) {
                response.data = json_result["listenKey"].asString();
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

    void BinanceSpotRestClient::keep_userDataStream(const std::string listenKey, CommonRestResponse<std::string> &response) {
        std::string url = this->baseUrl + "/api/v3/userDataStream";
        
        std::vector<std::string> extra_http_header;
        std::string header_chunk("X-MBX-APIKEY: ");
        header_chunk.append(this->apiKey);
        extra_http_header.push_back(header_chunk);

        std::string post_data = ("listenKey=" + listenKey);
        std::string action = "PUT";
        
        CURL* curl = curl_easy_init();
        CURLcode res;
        std::string str_result;

        try {
            res = curl_api_with_header(curl, url, str_result , extra_http_header, post_data, action);
        } catch (std::exception &e) {
            response.code = -500;
            response.msg = "parse json error: " + std::string(e.what());
            if (curl != nullptr) {
                curl_easy_cleanup(curl);
            }
            return;
        }

        if (str_result.size() > 0 ) {
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
