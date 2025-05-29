#ifndef _BINANCE_PARAM_H_
#define _BINANCE_PARAM_H_

#include <iostream>

using namespace std;

namespace binance {
    struct SpotNewOrder {
        string symbol;
        string side;
        string type;
        string timeInForce;
        double quantity;
        double price;
        double quoteOrderQty;
        string newClientOrderId;
        string newOrderRespType;
        uint64_t createTime;
    };
}

#endif