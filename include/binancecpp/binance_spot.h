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
        void setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response);

        void get_exchangeInfo(std::vector<std::string>& symbols, CommonRestResponse<std::vector<binance::SpotExchangeInfo>> &response);
        void get_account(CommonRestResponse<binance::SpotAccount> &response);
        void start_userDataStream(CommonRestResponse<std::string> &response);
        void keep_userDataStream(const std::string listenKey, CommonRestResponse<std::string> &response);

        void create_new_order(SpotNewOrder& order, CommonRestResponse<SpotNewOrderResult> &response);
    private:
        uint64_t get_property_timestamp();

    };
}
#endif