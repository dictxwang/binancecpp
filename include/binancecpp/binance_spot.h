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
        void get_account(CommonRestResponse<binance::SpotAccount> &response);
        void start_userDataStream(CommonRestResponse<std::string> &response);
        void keep_userDataStream(const std::string listenKey, CommonRestResponse<std::string> &response);

    protected:
        static unsigned long timeOffset;
        unsigned long calculate_timestamp();
    
    public:
        static void setServerTimeOffset(binance::CommonRestResponse<unsigned long> response);
    };
}
#endif