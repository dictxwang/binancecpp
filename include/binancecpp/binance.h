#ifndef _BINANCE_H_
#define _BINANCE_H_

#include <string>
#include <curl/curl.h>
#include "binance_model.h"
#include "json/json.h"

namespace binance {

    const std::pair<std::string, std::string> SPOT_API_URL = {"https://api.binance.com", "api.binance.com"};
    const std::pair<std::string, std::string> FUTURES_API_URL = {"https://fapi.binance.com", "fapi.binance.com"};
    const std::pair<std::string, std::string> FUTURES_INTERNAL_API_URL = {"https://fapi-mm.binance.com", "fapi-mm.binance.com"};
    const std::pair<std::string, std::string> OPTIONS_API_URL = {"https://eapi.binance.com", "eapi.binance.com"};
    const std::pair<std::string, std::string> PORTFOLIO_API_URL = {"https://papi.binance.com", "papi.binance.com"};

    enum MarketType {
        SPOT,
        FUTURES,
        OPTIONS,
        DELIVERY,
        PORTFOLIO,
    };

    template <typename T>
    struct CommonRestResponse {
        T data;
        int code;
        std::string msg;
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
                return FUTURES_API_URL;
            case PORTFOLIO:
                return PORTFOLIO_API_URL;
            default:
                return SPOT_API_URL;
        }
    }

    class BinanceRestClient {
        public:
            // Constructor
            BinanceRestClient() {}
    
            // Destructor
            virtual ~BinanceRestClient() {
            }
    
        protected:
            // Protected methods
            void setLocalIP(const std::string& localIP); // call before init
            void setRemoteIP(const std::string& remoteIP); // call before init
            void init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal = false);
    
            CURLcode curl_api(CURL* curl, std::string &url, std::string &result_json );
		    CURLcode curl_api_with_header(CURL* curl, std::string &url, std::string &result_json , std::vector <std::string> &extra_http_header, std::string &post_data, std::string &action );
		
        protected:
    
            std::string apiKey;
            std::string secretKey;
            std::string localIP;
            std::string remoteIP;
            bool useInternal;
            std::string baseUrl;
            std::string serverHost;
            std::string serverPort;
            MarketType marketType;
    };
}
#endif
