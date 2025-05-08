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
        long tradeId; // "t"
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

}
#endif