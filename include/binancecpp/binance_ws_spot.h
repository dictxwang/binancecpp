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

    protected:
        // std::string make_subscribe_frame(std::vector<std::string> &params) override;
    };
}
#endif