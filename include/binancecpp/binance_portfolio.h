#ifndef _BINANCE_PORTFOLIO_H_
#define _BINANCE_PORTFOLIO_H_

#include "binance.h"

/*
    portfolio
*/

namespace binance {

    class BinancePortfolioRestClient : public BinanceRestClient {
    public:
        // Constructor
        BinancePortfolioRestClient() : BinanceRestClient() {}

        // Destructor
        ~BinancePortfolioRestClient() {
        }


    public:
        // Public methods
        void init(const std::string& apiKey, const std::string& secretKey, bool useInternal = false);

        // Some api methods
    
    };
}

#endif