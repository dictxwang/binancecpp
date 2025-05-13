#ifndef _BINANCE_WS_MODEL_H_
#define _BINANCE_WS_MODEL_H_

#include <iostream>
#include "json/json.h"
#include "util/binance_tool.h"
#include "util/common_tool.h"

using namespace std;

namespace binance {

    struct WsLogonOnParam {
        string apiKey;
        string signature;
        uint64_t timestamp;
    };

    struct WsLogonOnReq {
        string id;
        string method;
        WsLogonOnParam params;
    };

    struct WsBookTickerEvent {
        uint64_t updateId;
        string symbol;
        double bestBidPrice;
        double bestBidQty;
        double bestAskPrice;
        double bestAskQty;
    };

    struct WsSpotBalanceUpdateEvent {
        string asset; //"a"
        double change; //""d
        uint64_t transactionTime; // "T"
    };

    struct WsAccountUpdateBalanceEvent {
        string asset; // "a"
        double free; // "f"
        double locked; // "l"
    };

    struct WsSpotAccountUpdateEvent {
        uint64_t updateTime; // "u"
        vector<WsAccountUpdateBalanceEvent> balances; // "B"
    };

    struct WsSpotOrderUpdateEvent {
        string symbol; // "s"
        string clientOrderId; // "c"
        string side; // "S"`
        string type; // "o"
        string timeInForce; // "f"
        double volume; // "q"
        double price; // "p"
        double stopPrice; //"P"
        double iceBergVolume; // "F"
        long orderListId; // "g"` for OCO
        string origCustomOrderId; // "C"`customized order ID for the original order
        string executionType; // "x" execution type for this event NEW/TRADE...
        string status; // "X"` order status
        string rejectReason; // "r" 
        uint64_t id; // "i" order id
        double latestVolume; // "l" quantity for the latest trade
        double filledVolume; // "z"
        double latestPrice; // "L" price for the latest trade
        string feeAsset; // "N"
        double feeCost; // "n"
        uint64_t transactionTime; // "T"
        uint64_t tradeId; // "t"
        // uint64_t ignoreI; // "I"
        bool isInOrderBook; // "w" is the order in the order book?
        bool isMaker; // "m" is this order maker?
        // bool ignoreM; // "M"` ignore
        uint64_t createTime; // "O"
        double filledQuoteVolume; // "Z" the quote volume that already filled
        double latestQuoteVolume; // "Y" the quote volume for the latest trade
        double quoteVolume; // "Q"`
        string selfTradePreventionMode; // "V"
    };

    WsBookTickerEvent convertJsonToWsBookTickerEvent(Json::Value &json_value);
    WsSpotBalanceUpdateEvent convertJsonToWsSpotBalanceUpdateEvent(Json::Value &json_value);
    WsAccountUpdateBalanceEvent convertJsonToWsAccountUpdateBalanceEvent(Json::Value &json_value);
    WsSpotAccountUpdateEvent convertJsonToWsWsSpotAccountUpdateEvent(Json::Value &json_value);
    WsSpotOrderUpdateEvent convertJsonToWsSpotOrderUpdateEvent(Json::Value &json_value);

    /* Start Futures */
    struct WsFuturesMarkPriceEvent {
        uint64_t eventTime;
        string symbol;
        double markPrice;
        double indexPrice;
        double estimatedSettlePrice; // only useful in the last hour before the settlement starts
        double fundingRate;
        uint64_t nextFundingTime;
    };

    struct WsFuturesBookTickerEvent {
        uint64_t eventTime;
        uint64_t transactionTime;
        uint64_t updateId;
        string symbol;
        double bestBidPrice;  // b
        double bestBidQty;  // B
        double bestAskPrice;  // a
        double bestAskQty;  // A
    };

    struct WsFuturesAccountUpdateBalanceEvent {
        string asset; // a
        double walletBalance; // wb
        double crossWalletBalance; // cw
        double balanceChange; // bc Balance Change except PnL and Commission
    };
    struct WsFuturesAccountUpdatePositionEvent {
        string symbol; // s
        double postionAmout; // pa
        double entryPrice; // ep
        double breakevenPrice; // ebp
        double accumulatedRealized; // cr (Pre-fee) Accumulated Realized
        double unrealizedPnL; // up Unrealized PnL
        string marginType; // mt
        double isolatedWallet;// iw Isolated Wallet (if isolated position)
        string positionSide; // ps 
    };

    struct WsFuturesAccountUpdateEvent {
        string eventType;  // e
        uint64_t eventTIme;  // E
        uint64_t transactionTime;  // T
        string eventReasonType; // m
        vector<WsFuturesAccountUpdateBalanceEvent> balances;
        vector<WsFuturesAccountUpdatePositionEvent> positions;
    };

    struct WsFuturesOrderTradeUpdateEvent {
        string eventType;  // e
        uint64_t eventTIme;  // E
        uint64_t transactionTime;  // T
        string symbol; // "s"
        string clientOrderId; // "c"
        string side; // "S"`
        string type; // "o"
        string timeInForce; // "f"
        double volume; // "q"
        double price; // "p"
        double averagePrice; // "ap"
        double stopPrice; // "sp"
        string executionType; // "x"
        string orderStatus; // "X"
        uint64_t id; // "i" order id
        double latestVolume; // "l" quantity for the latest trade
        double filledVolume; // "z"
        double latestPrice; // "L" price for the latest trade
        string feeAsset; // "N"
        double feeCost; // "n"
        uint64_t transactionTime; // "T"
        uint64_t tradeId; // "t"
        double bidNotional; // "b"
        double askNotional; // "a"
        bool isMaker; // "m"
        bool isReduce; // "R"
        string workingType; // "wt"
        string originalOrderType; // "ot"
        string positionSide; // "ps"
        bool isClosingPosition; // "cp" If Close-All, pushed with conditional order
        double activationPrice; // "AP"`  // Activation Price, only puhed with TRAILING_STOP_MARKET order
        double callbackRate; // "cr"`  // Callback Rate, only puhed with TRAILING_STOP_MARKET order
	    bool priceProtect; // "pP"`  // If price protection is turned on
        double realizedPnL; // "rp"`  // Realized Profit of the trade
	    string sTP; // "V"`   // STP mode
	    string priceMode; // "pm"`  // Price match mode
	    uint64_t gTD; // "gtd"` // TIF GTD order auto cancel time
    };
    
    WsFuturesMarkPriceEvent convertJsonToWsFuturesMarkPriceEvent(Json::Value &json_value);
    WsFuturesBookTickerEvent convertJsonToWsFuturesBookTickerEvent(Json::Value &json_value);
    WsFuturesAccountUpdateEvent convertJsonToWsFuturesAccountUpdateEvent(Json::Value &json_value);
    WsFuturesOrderTradeUpdateEvent convertJsonToWsFuturesOrderTradeUpdateEvent(Json::Value &json_value);
    /* End Futures */

}
#endif