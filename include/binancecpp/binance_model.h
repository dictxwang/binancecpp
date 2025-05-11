#ifndef _BINANCE_MODEL_H_
#define _BINANCE_MODEL_H_

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

namespace binance {

    template <typename T>
    struct CommonRestResponse {
        T data;
        int code = 0;
        string msg;
    };

    /* Start: structs of common */
    struct OrderRateLimit {
        string rateLimitType;
        string interval;
        int intervalNum;
        int limit;
    };
    /* End: structs of common */

    /* Start: structs of spot */
    struct SpotExchangeInfo {
        string symbol;
        string status;
        string baseAsset;
        string quoteAsset;
        vector<string> orderTypes;
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
        string asset;
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
        string accountType;
        vector<BalanceLite> balances;
        vector<string> permissions;
        uint64_t uid;
    };
    /* End: structs of spot */

    /* Start: structs of futures */
    struct FuturesExchangeInfo {
        string symbol;
        string pair;
        string contractType;
        string status;
        string baseAsset;
        string quoteAsset;
        vector<string> orderTypes;
        vector<string> timeInForces;
        double minPrice;
        double maxPrice;
        double tickSize;
        double minQty;
        double maxQty;
        double stepSize;
    };


    struct FuturesAccountAsset {
        string asset;
        double walletBalance;
        double unrealizedProfit;
        double marginBalance;
        double maintMargin;
        double initialMargin;
        double positionInitialMargin;
        double openOrderInitialMargin;
        double crossWalletBalance;
        double crossUnPnl;
        double availableBalance;
        double maxWithdrawAmount;
        bool marginAvailable;
        uint64_t updateTime;
    };

    struct FuturesAccountPosition {
        string symbol;
        double initialMargin;
        double maintMargin;
        double unrealizedProfit;
        double positionInitialMargin;
        double openOrderInitialMargin;
        int leverage;
        bool isolated;
        double entryPrice;
        double maxNotional;
        double bidNotional;  // bids notional, ignore
        double askNotional;  // ask notional, ignore
        string positionSide;
        double positionAmt;
        uint64_t updateTime;
    };

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/account/rest-api/Account-Information-V2
    struct FuturesAccount {
        int feeTier;  // account commission tier 
        bool feeBurn;
        bool canTrade;
        bool canDeposit;
        bool canWithdraw;
        uint64_t updateTime;  // reserved property, please ignore 
        bool multiAssetsMargin;
        int tradeGroupId;
        double totalInitialMargin;  // total initial margin required with current mark price (useless with isolated positions)
        double totalMaintMargin;  // total maintenance margin required
        double totalWalletBalance;  // total wallet balance
        double totalUnrealizedProfit;  // total wallet balance
        double totalMarginBalance;  // total margin balance
        double totalPositionInitialMargin;  // initial margin required for positions with current mark price
        double totalOpenOrderInitialMargin;  // initial margin required for open orders with current mark price
        double totalCrossWalletBalance;  // crossed wallet balance,
        double totalCrossUnPnl;  // unrealized profit of crossed positions
        double availableBalance;  // available balance
        double maxWithdrawAmount;  // maximum amount for transfer out
        vector<FuturesAccountAsset> assets;
        vector<FuturesAccountPosition> positions;
    };

    struct FuturesCommissionRate {
        string symbol;
        double makerCommissionRate;
        double takerCommissionRate;
    };

    struct FuturesAccountConfig {
        int feeTier;  // account commission tier
        bool canTrade;
        bool canDeposit;
        bool canWithdraw;
        bool dualSidePosition;
        uint64_t updateTime; // reserved property, please ignore
        bool multiAssetsMargin;
        int tradeGroupId;
    };

    struct FuturesSymbolConfig {
        string symbol;
        string marginType;
        bool isAutoAddMargin;
        int leverage;
        double maxNotionalValue;
    };

    struct FuturesChangeLeverageResult {
        string symbol;
        double maxNotionalValue;
        int leverage;
    };

    /* End: structs of futures */
}

#endif