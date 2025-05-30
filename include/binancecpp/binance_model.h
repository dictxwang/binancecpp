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

    struct SpotNewOrderResult {
        string symbol;
        uint64_t orderId;
        long orderListId;
        string clientOrderId;
        uint64_t transactTime;
        double price;
        double origQty;
        double executedQty;
        double origQuoteOrderQty;
        double cummulativeQuoteQty;
        string status;
        string timeInForce;
        string type;
        string side;
        uint64_t workingTime;
        string selfTradePreventionMode;
    };
    /* End: structs of spot */

    /* Start: structs of wallet */
    struct WalletsDutfTranserResult {
        string fromAsset;
        double amount;
        uint64_t operateTime;
        double serviceChargeAmount;
        double transferedAmount;
        uint64_t tranId;
    };
    struct WalletsDutfTranserResp {
        double totalServiceCharge = 0;
        double totalTransfered = 0;
        vector<WalletsDutfTranserResult> transferResult;
    };
    /* End: structs of wallet */

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

    struct FuturesPositionRisk {
        string symbol;
        double positionAmt;
        double entryPrice;
        double breakEvenPrice;
        double markPrice;
        double unRealizedProfit;
        double liquidationPrice;
        int leverage;
        double maxNotionalValue;
        string marginType;
        double isolatedMargin;
        bool isAutoAddMargin;
        string positionSide;
        double notional;
        double isolatedWallet;
        uint64_t updateTime;
    };

    struct FuturesSymbolPriceTicker {
        string symbol;
        double price;
        uint64_t time;
    };

    struct FuturesPremiumIndex {
        string symbol;
        double markPrice;
        double indexPrice;
        double estimatedSettlePrice;
        double lastFundingRate;
        double interestRate;
        uint64_t nextFundingTime;
        uint64_t time;
    };

    /* End: structs of futures */
}

#endif