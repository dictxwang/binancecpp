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
        bool startBookTickerV1(std::vector<std::string>& symbols);
        bool startAllBookTickersV1();

        void initMarkPriceV1(bool useInternal, bool useCombine);
        bool startMarkPriceV1(std::vector<std::string>& symbols, binance::WsMarkPriceInterval interval);
        bool startAllMarkPricesV1(binance::WsMarkPriceInterval interval);

        void initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStreamV1(std::string listenKey);
        // TODO: unavailable
        void initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStream();

        void initOrderService(std::string apiKey, std::string secretKey, bool useInternal);
        bool startOrderService();
        bool placeOrder(binance::FuturesNewOrder &order);
        bool cancelOrder(binance::FuturesCancelOrder &order);
    };
}