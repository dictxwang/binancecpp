#ifndef _BINANCE_SPOT_H_
#define _BINANCE_SPOT_H_

#include "binance.h"
#include <iostream>

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
        void get_exchangeInfo(binance::CommonRestResponse<binance::ExchangeInfoResponse> &response);
        void init(const std::string& apiKey, const std::string& secretKey, bool useInternal = false);
    };
}
#endif