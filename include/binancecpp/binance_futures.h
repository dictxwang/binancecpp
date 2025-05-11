#ifndef _BINANCE_FUTURES_H_
#define _BINANCE_FUTURES_H_

#include "binance.h"

/*
    USD-M Futures
*/

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
            void setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response);

            void get_exchangeInfo(CommonRestResponse<std::vector<binance::FuturesExchangeInfo>> &response);
            void get_account_v2(CommonRestResponse<binance::FuturesAccount> &response);
            void get_commissionRate(std::string &symbol, CommonRestResponse<binance::FuturesCommissionRate> &response);
            void get_accountConfig(CommonRestResponse<binance::FuturesAccountConfig> &response);
            void get_symbolConfig(std::string &symbol, CommonRestResponse<binance::FuturesSymbolConfig> &response);
            void get_orderRateLimit(CommonRestResponse<std::vector<binance::OrderRateLimit>> &response);
            void get_multiAssetMargin(CommonRestResponse<bool> &response);
            void get_positionSideDual(CommonRestResponse<bool> &response);
            void get_bnbFeeBurn(CommonRestResponse<bool> &response);
            void toggle_bnbFeeBurn(bool feeBurn, CommonRestResponse<void> &response);
        
        private:
            uint64_t get_property_timestamp();
    };
}
#endif