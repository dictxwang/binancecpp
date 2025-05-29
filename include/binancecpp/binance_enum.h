#ifndef _BINANCE_ENUM_H_
#define _BINANCE_ENUM_H_

using namespace std;

namespace binance {

    enum ApiSecretType {
        SecTypeNone,
        SecTypeApiKey,
        SecTypeSignature,
    };

    enum MarketType {
        WALLET,
        SPOT,
        FUTURES,
        OPTIONS,
        DELIVERY,
        PORTFOLIO,
    };

    enum WsMarkPriceInterval {
        WsMPThreeSeconds,
        WsMPOneSecond,
    };

    const string MarginType_ISOLATED = "ISOLATED";
    const string MarginType_CROSSED = "CROSSED";

    const string PositionSide_BOTH = "BOTH";
    const string PositionSide_LONG = "LONG";
    const string PositionSide_SHORT = "SHORT";

    const string ORDER_SIDE_BUY = "BUY";
    const string ORDER_SIDE_SELL = "SELL";

    const string ORDER_TYPE_LIMIT = "LIMIT";
    const string ORDER_TYPE_STOP = "STOP";
    const string ORDER_TYPE_STOP_MARKET = "STOP_MARKET";
    const string ORDER_TYPE_TAKE_PROFIT = "TAKE_PROFIT";
    const string ORDER_TYPE_TAKE_PROFIT_MARKET = "TAKE_PROFIT_MARKET";
    const string ORDER_TYPE_TRAILING_STOP_MARKET = "TRAILING_STOP_MARKET";
    const string ORDER_TYPE_LIQUIDATION = "LIQUIDATION";

    const string ORDER_STATUS_NEW = "NEW";
    const string ORDER_STATUS_PARTIALLY_FILLED = "PARTIALLY_FILLED";
    const string ORDER_STATUS_FILLED = "FILLED";
    const string ORDER_STATUS_CANCELED = "CANCELED";
    const string ORDER_STATUS_EXPIRED = "EXPIRED";
    const string ORDER_STATUS_EXPIRED_IN_MATCH = "EXPIRED_IN_MATCH";

    const string ORDER_EXECUTION_TYPE_NEW = "NEW";
    const string ORDER_EXECUTION_TYPE_CANCELED = "CANCELED";
    const string ORDER_EXECUTION_TYPE_CALCULATED = "CALCULATED";
    const string ORDER_EXECUTION_TYPE_EXPIRED = "EXPIRED";
    const string ORDER_EXECUTION_TYPE_TRADE = "TRADE";
    const string ORDER_EXECUTION_TYPE_AMENDMENT = "AMENDMENT";

    const string ORDER_WORKING_TYPE_MARK_PRICE = "MARK_PRICE";
    const string ORDER_WORKING_TYPE_CONTRACT_PRICE = "CONTRACT_PRICE";

    const string ORDER_RESP_TYPE_ACK = "ACK"; // DEFAULT
    const string ORDER_RESP_TYPE_RESULT = "RESULT";

    const string TimeInForce_GTC = "GTC";
    const string TimeInForce_IOC = "IOC";
    const string TimeInForce_FOK = "FOK";
    const string TimeInForce_GTX = "GTX";

    const int Isolated_Position_Type_Add = 1;
    const int Isolated_Position_Type_Reduce = 2;

    const string FuturesUserDataAccountUpdate = "ACCOUNT_UPDATE";
    const string FuturesUserDataOrderTradeUpdate = "ORDER_TRADE_UPDATE";

    const int RestCodeOK = 0;
    const int WsCallbackStatusOK = 200;
}

#endif