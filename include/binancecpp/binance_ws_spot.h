#ifndef _BINANCE_WS_SPOT_H_
#define _BINANCE_WS_SPOT_H_

#include "binance_ws.h"

namespace binance {

    class BinanceSpotWsClient : public BinanceWsClient {

    public:
        BinanceSpotWsClient() {}
        ~BinanceSpotWsClient() {}
    
        void initBookTicker(bool useInternal, bool useCombine);
        bool startBookTicker(WS_CB customCallback, std::vector<std::string>& symbols);

        void initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStreamV1(WS_CB customCallback, std::string listenKey);
        void initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStream(WS_CB customCallback);

    protected:
    };
}
#endif