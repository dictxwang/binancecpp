#ifndef _BINANCE_WALLET_H_
#define _BINANCE_WALLET_H_

#include "binance.h"

namespace binance {
    
    class BinanceWalletRestClient : public BinanceRestClient {
    public:
        // Constructor
        BinanceWalletRestClient() : BinanceRestClient() {}

        // Destructor
        ~BinanceWalletRestClient() {
        }


    public:
        // Public methods
        void init(const std::string& apiKey, const std::string& secretKey, bool useInternal = false);
    
        // TODO universal transfer 
        // https://developers.binance.com/docs/wallet/asset/user-universal-transfer
    private:
        uint64_t get_property_timestamp();
    };
}
#endif