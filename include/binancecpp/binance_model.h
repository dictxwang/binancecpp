#ifndef _BINANCE_MODEL_H_
#define _BINANCE_MODEL_H_

#include <iostream>

namespace binance {

    struct ExchangeInfo {
        std::string symbol;
        std::string status;
        std::string baseAsset;
        std::string quoteAsset;
        double minPrice;
        double maxPrice;
        double tickSize;
        double stepSize;
    };

    struct ExchangeInfoResponse {
        std::vector<ExchangeInfo> symbols;
        // std::string timezone;
        // std::string serverTime;
        // std::string rateLimits;
        // std::string exchangeFilters;
    };
}
#endif