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

    const string ACCOUNT_TYPE_SPOT = "SPOT";
    const string ACCOUNT_TYPE_MARGIN = "MARGIN";

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

    // https://developers.binance.com/docs/wallet/asset/user-universal-transfer
    const string UT_MAIN_UMFUTURE = "MAIN_UMFUTURE";
    const string UT_MAIN_CMFUTURE = "MAIN_CMFUTURE";
    const string UT_MAIN_MARGIN = "MAIN_MARGIN";
    const string UT_UMFUTURE_MAIN = "UMFUTURE_MAIN";
    const string UT_UMFUTURE_MARGIN = "UMFUTURE_MARGIN";
    const string UT_CMFUTURE_MAIN = "CMFUTURE_MAIN";
    const string UT_CMFUTURE_MARGIN = "CMFUTURE_MARGIN";
    const string UT_MARGIN_MAIN = "MARGIN_MAIN";
    const string UT_MARGIN_UMFUTURE = "MARGIN_UMFUTURE";
    const string UT_MARGIN_CMFUTURE = "MARGIN_CMFUTURE";
    const string UT_ISOLATEDMARGIN_MARGIN = "ISOLATEDMARGIN_MARGIN";
    const string UT_MARGIN_ISOLATEDMARGIN = "MARGIN_ISOLATEDMARGIN";
    const string UT_ISOLATEDMARGIN_ISOLATEDMARGIN = "ISOLATEDMARGIN_ISOLATEDMARGIN";
    const string UT_MAIN_FUNDING = "MAIN_FUNDING";
    const string UT_FUNDING_MAIN = "FUNDING_MAIN";
    const string UT_FUNDING_UMFUTURE = "FUNDING_UMFUTURE";
    const string UT_UMFUTURE_FUNDING = "UMFUTURE_FUNDING";
    const string UT_MARGIN_FUNDING = "MARGIN_FUNDING";
    const string UT_FUNDING_MARGIN = "FUNDING_MARGIN";
    const string UT_FUNDING_CMFUTURE = "FUNDING_CMFUTURE";
    const string UT_CMFUTURE_FUNDING = "CMFUTURE_FUNDING";
    const string UT_MAIN_OPTION = "MAIN_OPTION";
    const string UT_OPTION_MAIN = "OPTION_MAIN";
    const string UT_UMFUTURE_OPTION = "UMFUTURE_OPTION";
    const string UT_OPTION_UMFUTURE = "OPTION_UMFUTURE";
    const string UT_MARGIN_OPTION = "MARGIN_OPTION";
    const string UT_OPTION_MARGIN = "OPTION_MARGIN";
    const string UT_FUNDING_OPTION = "FUNDING_OPTION";
    const string UT_OPTION_FUNDING = "OPTION_FUNDING";
    const string UT_MAIN_PORTFOLIO_MARGIN = "MAIN_PORTFOLIO_MARGIN";
    const string UT_PORTFOLIO_MARGIN_MAIN = "PORTFOLIO_MARGIN_MAIN";
}

#endif