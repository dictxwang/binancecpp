#include "binancecpp/binance_wallet.h"

namespace binance {
    
    void BinanceWalletRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::WALLET, useInternal);
    }

    void BinanceWalletRestClient::setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response) {

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

    uint64_t BinanceWalletRestClient::get_property_timestamp() {
        
        return get_current_ms_epoch() - this->timeOffset;
    }

    void BinanceWalletRestClient::universal_transfer(WalletUniversalTransfer& param, CommonRestResponse<uint64_t>& response) {
        std::string url = this->serverMeta.baseUrl + "/sapi/v1/asset/transfer";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));
        body_params.push_back("type=" + param.type);
        body_params.push_back("asset=" + param.asset);
        body_params.push_back("amount=" + strHelper::toString(param.amount));
        if (param.fromSymbol.size() > 0) {
            body_params.push_back("fromSymbol=" + strHelper::toString(param.fromSymbol));
        }
        if (param.toSymbol.size() > 0) {
            body_params.push_back("toSymbol=" + strHelper::toString(param.toSymbol));
        }

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

            if (json_result.isMember("tranId")) {
                response.data = json_result["tranId"].asUInt64();
            }
        }
        return;
    }

    void BinanceWalletRestClient::dust_transfer(WalletDustfTranser& param, CommonRestResponse<WalletsDutfTranserResp>& response) {
        std::string url = this->serverMeta.baseUrl + "/sapi/v1/asset/dust";

        binance::CommonRestResponse<std::string> action_response;
        std::string action = "POST";
        std::vector <std::string> empty;
        std::vector <std::string> query_params;
        std::vector <std::string> body_params;
        query_params.push_back("timestamp=" + std::to_string(get_property_timestamp()));

        string asset = strHelper::joinStrings(param.assets, ",");
        body_params.push_back("asset=" + asset);
        if (param.accountType.size() > 0) {
            body_params.push_back("accountType=" + param.accountType);
        }

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

            response.data.totalServiceCharge = str_to_dobule(json_result["totalServiceCharge"]);
            response.data.totalTransfered = str_to_dobule(json_result["totalTransfered"]);
            if (json_result.isMember("transferResult")) {
                Json::Value results = json_result["transferResult"];
                for (int i = 0; i < results.size(); i++) {
                    WalletsDutfTranserResult wdtr;
                    wdtr.fromAsset = results[i]["fromAsset"].asString();
                    wdtr.amount = str_to_dobule(results[i]["amount"]);
                    wdtr.serviceChargeAmount = str_to_dobule(results[i]["serviceChargeAmount"]);
                    wdtr.transferedAmount = str_to_dobule(results[i]["transferedAmount"]);
                    wdtr.tranId = results[i]["tranId"].asUInt64();
                    wdtr.operateTime = results[i]["operateTime"].asUInt64();

                    response.data.transferResult.push_back(wdtr);
                }
            }
        }
        return;
    }
}