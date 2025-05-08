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
}

#endif