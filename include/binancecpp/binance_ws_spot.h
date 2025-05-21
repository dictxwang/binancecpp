#ifndef _BINANCE_WS_SPOT_H_
#define _BINANCE_WS_SPOT_H_

#include "binance_ws.h"

namespace binance {

    class BinanceSpotWsClient : public BinanceWsClient {

    public:
        BinanceSpotWsClient() {}
        ~BinanceSpotWsClient() {}
    
        void initBookTicker(bool useInternal, bool useCombine);
        bool startBookTicker(std::vector<std::string>& symbols);

        void initUserDataStreamV1(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStreamV1(std::string listenKey);
        void initUserDataStream(std::string apiKey, std::string secretKey, bool useInternal);
        bool startUserDataStream();
    };
}
#endif