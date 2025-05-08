#ifndef _BINANCE_MODEL_H_
#define _BINANCE_MODEL_H_

#include <iostream>

namespace binance {

    template <typename T>
    struct CommonRestResponse {
        T data;
        int code = 0;
        std::string msg;
    };

    struct SpotExchangeInfo {
        std::string symbol;
        std::string status;
        std::string baseAsset;
        std::string quoteAsset;
        std::vector<std::string> orderTypes;
        double minPrice;
        double maxPrice;
        double tickSize;
        double minQty;
        double maxQty;
        double stepSize;
    };

    struct CommissionRates {
        double maker;
        double taker;
        double buyer;
        double seller;
    };

    struct BalanceLite {
        std::string asset;
        double free;
        double locked;
    };

    struct SpotAccount {
        int makerCommission;
        int takerCommission;
        int buyerCommission;
        int sellerCommission;
        CommissionRates commissionRates;
        bool canTrade;
        bool canWithdraw;
        bool canDeposit;
        bool brokered;
        bool requireSelfTradePrevention;
        bool preventSor;
        uint64_t updateTime;
        std::string accountType;
        std::vector<BalanceLite> balances;
        std::vector<std::string> permissions;
        uint64_t uid;
    };

    struct FuturesExchangeInfo {
        std::string symbol;
        std::string pair;
        std::string contractType;
        std::string status;
        std::string baseAsset;
        std::string quoteAsset;
        std::vector<std::string> orderTypes;
        std::vector<std::string> timeInForces;
        double minPrice;
        double maxPrice;
        double tickSize;
        double minQty;
        double maxQty;
        double stepSize;
    };
}
#endif