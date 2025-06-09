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
            void init(const std::string apiKey, const std::string secretKey, bool useInternal = false);
            void setServerTimeOffset(binance::CommonRestResponse<uint64_t> &response);

            void get_exchangeInfo(CommonRestResponse<std::vector<binance::FuturesExchangeInfo>> &response);
            void get_account_v2(CommonRestResponse<binance::FuturesAccount> &response);
            void get_commissionRate(const std::string symbol, CommonRestResponse<binance::FuturesCommissionRate> &response);
            void get_accountConfig(CommonRestResponse<binance::FuturesAccountConfig> &response);
            void get_symbolConfig(const std::string symbol, CommonRestResponse<binance::FuturesSymbolConfig> &response);
            void get_orderRateLimit(CommonRestResponse<std::vector<binance::OrderRateLimit>> &response);
            void get_multiAssetMargin(CommonRestResponse<bool> &response);
            void get_positionSideDual(CommonRestResponse<bool> &response);
            void get_bnbFeeBurn(CommonRestResponse<bool> &response);
            void get_positionRiskV2(const std::string symbol, CommonRestResponse<std::vector<FuturesPositionRisk>> &response);
            void get_tickerPriceV2(const std::string symbol, CommonRestResponse<std::vector<FuturesSymbolPriceTicker>> &response);
            void get_premiumIndex(const std::string symbol, CommonRestResponse<std::vector<FuturesPremiumIndex>> &response);

            void toggle_bnbFeeBurn(bool feeBurn, CommonRestResponse<bool> &response);
            void change_marginType(const std::string symbol, const std::string marginType, CommonRestResponse<bool> &response);
            void change_positionSideDual(bool dualSidePosition, CommonRestResponse<bool> &response);
            void change_initialLeverage(const std::string symbol, int leverage, CommonRestResponse<FuturesChangeLeverageResult> &response);
            void change_multiAssetsMargin(bool multiAssetsMargin, CommonRestResponse<bool> &response);
            void modify_isolatedPositionMargin(const std::string symbol, const std::string positionSide, double amount, int type, CommonRestResponse<bool> &response);

            void start_userDataStream(CommonRestResponse<std::string> &response);
            void keep_userDataStream(const std::string listenKey, CommonRestResponse<std::string> &response);
        
        private:
            uint64_t get_property_timestamp();
    };
}
#endif