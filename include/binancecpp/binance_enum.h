#ifndef _BINANCE_ENUM_H_
#define _BINANCE_ENUM_H_

namespace binance {

    enum ApiSecretType {
        SecTypeNone,
        SecTypeApiKey,
        SecTypeSignature,
    };

    enum MarketType {
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

    const int Isolated_Position_Type_Add = 1;
    const int Isolated_Position_Type_Reduce = 2;
}

#endif