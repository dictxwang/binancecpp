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
        bool startBookTickerV1(WS_CB customCallback, std::vector<std::string>& symbols);
        bool startAllBookTickersV1(WS_CB customCallback);

        void initMarkPriceV1(bool useInternal, bool useCombine);
        bool startMarkPriceV1(WS_CB customCallback, std::vector<std::string>& symbols, binance::WsMarkPriceInterval interval);
        bool startAllMarkPricesV1(WS_CB customCallback, binance::WsMarkPriceInterval interval);

        void initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStreamV1(WS_CB customCallback, std::string listenKey);
        // TODO: unavailable
        void initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStream(WS_CB customCallback);

        void initOrderService(std::string apiKey, std::string secretKey, bool useInternal);
        bool startOrderService(WS_CB customCallback);
        bool placeOrder(binance::FuturesNewOrder &order);
        bool cancelOrder(binance::FuturesCancelOrder &order);
    };
}