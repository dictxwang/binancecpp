#ifndef _BINANCE_WS_FUTURES_H_
#define _BINANCE_WS_FUTURES_H_
#endif

#include "binance_ws.h"

namespace binance {

    class BinanceFuturesWsClient : public BinanceWsClient {

    public:
        BinanceFuturesWsClient() {}
        ~BinanceFuturesWsClient() {}
    
        void initBookTickerV1(bool useInternal, bool useCombine);
        std::pair<bool, string> startBookTickerV1(std::vector<std::string>& symbols);
        std::pair<bool, string> startAllBookTickersV1();

        void initMarkPriceV1(bool useInternal, bool useCombine);
        std::pair<bool, string> startMarkPriceV1(std::vector<std::string>& symbols, binance::WsMarkPriceInterval interval);
        std::pair<bool, string> startAllMarkPricesV1(binance::WsMarkPriceInterval interval);

        void initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal);
        std::pair<bool, string> startUserDataStreamV1(std::string listenKey);
        // TODO: unavailable
        void initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal);
        std::pair<bool, string> startUserDataStream();

        void initOrderService(std::string apiKey, std::string secretKey, bool useInternal);
        std::pair<bool, string> startOrderService();
        std::pair<bool, string> placeOrder(binance::FuturesNewOrder &order);
        std::pair<bool, string> cancelOrder(binance::FuturesCancelOrder &order);
    };
}