#ifndef _BINANCE_WS_PARAM_H_
#define _BINANCE_WS_PARAM_H_

#include <iostream>

using namespace std;

namespace binance {
    struct FuturesNewOrder {
        string symbol;
        string side;
        string positionSide;
        string type;
        string timeInForce;
        double quantity;
        double price;
        string reduceOnly;
        string newClientOrderId;
        string newOrderRespType;
        uint64_t createTime;

    };
    struct FuturesCancelOrder {
        string symbol;
        uint64_t orderId;
        string origClientOrderId;
    };
}
#endif