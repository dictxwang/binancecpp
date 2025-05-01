#ifndef _BINANCE_H_
#define _BINANCE_H_

#include <string>
#include <curl/curl.h>

namespace binance {

    const std::string SPOT_API_URL = "https://api.binance.com";
    const std::string FUTURES_API_URL = "https://fapi.binance.com";
    const std::string FUTURES_INTERNAL_API_URL = "https://fapi-mm.binance.com";
    const std::string OPTIONS_API_URL = "https://eapi.binance.com";
    const std::string PORTFOLIO_API_URL = "https://papi.binance.com";

    enum MarketType {
        SPOT,
        FUTURES,
        OPTIONS,
        DELIVERY,
        PORTFOLIO,
    };

    static std::string getBaseUrl(MarketType marketType, bool useInternal) {
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
    
        protected:
    
            std::string apiKey;
            std::string secretKey;
            std::string localIP;
            std::string remoteIP;
            bool useInternal;
            std::string baseUrl;
            MarketType marketType;
    };
}
#endif
