#ifndef _BINANCE_PARAM_H_
#define _BINANCE_PARAM_H_

#include <iostream>
#include <vector>

using namespace std;

namespace binance {

    /* Start Spot */
    struct SpotNewOrder {
        string symbol;
        string side;
        string type;
        string timeInForce;
        double quantity = 0;
        double price = 0;
        double quoteOrderQty = 0;
        string newClientOrderId;
        string newOrderRespType;
        uint64_t createTime = 0;
    };
    /* End Spot */

    /* Start Wallet */
    struct WalletUniversalTransfer {
        string type;
        string asset;
        double amount;
        string fromSymbol;
        string toSymbol;
    };

    struct WalletDustfTranser {
        vector<string> assets;
        string accountType;
    };
    /* End Wallet */
}

#endif