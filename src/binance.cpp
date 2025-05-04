#include "binancecpp/binance.h"

namespace binance {
    
    void BinanceRestClient::setLocalIP(const std::string& localIP) {
        this->localIP = localIP;
    }
    void BinanceRestClient::setRemoteIP(const std::string& remoteIP) {
        this->remoteIP = remoteIP;
    }
    void BinanceRestClient::init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal) {
        this->apiKey = apiKey;
        this->secretKey = secretKey;
        this->marketType = marketType;
        this->useInternal = useInternal;
        this->baseUrl = getBaseUrl(marketType, useInternal);
    }
}