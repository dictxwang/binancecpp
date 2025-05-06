#ifndef _BINANCE_SPOT_H_
#define _BINANCE_SPOT_H_

#include "binance.h"

namespace binance {
    
    class BinanceSpotRestClient : public BinanceRestClient {
    public:
        // Constructor
        BinanceSpotRestClient() : BinanceRestClient() {}

        // Destructor
        ~BinanceSpotRestClient() {
        }


    public:
        // Public methods
        void init(const std::string& apiKey, const std::string& secretKey, bool useInternal = false);
        void get_exchangeInfo(std::vector<std::string>& symbols, CommonRestResponse<std::vector<binance::SpotExchangeInfo>> &response);
    };
}
#endif