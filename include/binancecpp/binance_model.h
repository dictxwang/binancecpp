#ifndef _BINANCE_MODEL_H_
#define _BINANCE_MODEL_H_

#include <iostream>

namespace binance {

    template <typename T>
    struct CommonRestResponse {
        T data;
        int code;
        std::string msg;
    };

    struct SpotExchangeInfo {
        std::string symbol;
        std::string status;
        std::string baseAsset;
        std::string quoteAsset;
        double minPrice;
        double maxPrice;
        double tickSize;
        double minQty;
        double maxQty;
        double stepSize;
    };
}
#endif