#ifndef _BINANCE_DELIVERY_H_
#define _BINANCE_DELIVERY_H_

#include "binance.h"

/*
    COIN-M Futures
*/

namespace binance {

    class BinanceDeliveryRestClient : public BinanceRestClient {
    public:
        // Constructor
        BinanceDeliveryRestClient() : BinanceRestClient() {}

        // Destructor
        ~BinanceDeliveryRestClient() {
        }


    public:
        // Public methods
        void init(const std::string apiKey, const std::string secretKey, bool useInternal = false);
        void setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response);

        // Some api methods
    
    private:
        uint64_t get_property_timestamp();
    };
}

#endif