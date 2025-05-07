#ifndef _BINANCE_FUTURES_H_
#define _BINANCE_FUTURES_H_

#include "binance.h"

namespace binance {

    class BinanceFuturesRestClient : public BinanceRestClient {
        public:
            // Constructor
            BinanceFuturesRestClient() : BinanceRestClient() {}
    
            // Destructor
            ~BinanceFuturesRestClient() {
            }
    
    
        public:
            // Public methods
            void init(const std::string& apiKey, const std::string& secretKey, bool useInternal = false);
            void get_exchangeInfo(CommonRestResponse<std::vector<binance::FuturesExchangeInfo>> &response);
    };
}
#endif