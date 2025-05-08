#ifndef _BINANCE_H_
#define _BINANCE_H_

#include <string>
#include <curl/curl.h>
#include "binance_model.h"
#include "binance_ws_model.h"
#include "binance_enum.h"
#include "json/json.h"
#include "util/common_tool.h"
#include "util/binance_tool.h"
#include "util/string_helper.h"

namespace binance {

    const std::pair<std::string, std::string> SPOT_API_URL = {"https://api.binance.com", "api.binance.com"};
    const std::pair<std::string, std::string> FUTURES_API_URL = {"https://fapi.binance.com", "fapi.binance.com"};
    const std::pair<std::string, std::string> FUTURES_INTERNAL_API_URL = {"https://fapi-mm.binance.com", "fapi-mm.binance.com"};
    const std::pair<std::string, std::string> OPTIONS_API_URL = {"https://eapi.binance.com", "eapi.binance.com"};
    const std::pair<std::string, std::string> DELIVERY_API_URL = {"https://dapi.binance.com", "dapi.binance.com"};
    const std::pair<std::string, std::string> PORTFOLIO_API_URL = {"https://papi.binance.com", "papi.binance.com"};

    struct RestServerMeta {
        std::string baseUrl;
        bool useInternal;
        std::string serverHost;
        std::string serverPort;
        std::string localIP;
        std::string remoteIP;
    };

    static std::pair<std::string, std::string> getBaseUrl(MarketType marketType, bool useInternal) {
        switch (marketType) {
            case SPOT:
                return SPOT_API_URL;
            case FUTURES:
                if (useInternal) {
                    return FUTURES_INTERNAL_API_URL;
                } else {
                    return FUTURES_API_URL;
                }
            case OPTIONS:
                return OPTIONS_API_URL;
            case DELIVERY:
                return DELIVERY_API_URL;
            case PORTFOLIO:
                return PORTFOLIO_API_URL;
            default:
                return SPOT_API_URL;
        }
    }

    static size_t curl_write_callback(void* contents, size_t size, size_t nmemb, std::string* userData) {
        size_t totalSize = size * nmemb;
        userData->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    template <typename T>
    static bool parse_api_has_error(Json::Value& json_value, CommonRestResponse<T> &response) {
        if (json_value.isMember("code")) {
            int code = json_value["code"].asInt();
            if (code != 0) {
                response.code = code;
                if (json_value.isMember("msg")) {
                    response.msg = json_value["msg"].asString();
                }
                return true;
            }
        }
        return false;
    }

    class BinanceRestClient {
        public:
            // Constructor
            BinanceRestClient() {
                this->serverMeta = binance::RestServerMeta{};
            }
    
            // Destructor
            virtual ~BinanceRestClient() {
            }

        public:
            // Public methods
            void setLocalIP(const std::string& localIP); // call before init if need
            void setRemoteIP(const std::string& remoteIP); // call before init if need

        protected:
            // Protected methods
            void init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal = false);

            CURLcode curl_api(CURL* curl, std::string &url, std::string &result_json );
		    CURLcode curl_api_with_header(CURL* curl, std::string &url, std::string &result_json , std::vector <std::string> &extra_http_header, std::string &post_data, std::string &action );
		
        protected:
        
            std::string apiKey;
            std::string secretKey;
            binance::RestServerMeta serverMeta;
            MarketType marketType;

        public:
            long timeOffset = 0;
    };
}
#endif
