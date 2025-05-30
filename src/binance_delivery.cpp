#include "binancecpp/binance_delivery.h"
#include "binancecpp/json/json.h"

namespace binance {

    void BinanceDeliveryRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::DELIVERY, useInternal);
    }

    void BinanceDeliveryRestClient::setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response) {

        std::string url = this->serverMeta.baseUrl + "/dapi/v1/time";
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

    uint64_t BinanceDeliveryRestClient::get_property_timestamp() {
        
        return get_current_ms_epoch() - this->timeOffset;
    }

}