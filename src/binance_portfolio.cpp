#include "binancecpp/binance_portfolio.h"

/*
    portfolio
*/

namespace binance {

    void BinancePortfolioRestClient::init(const std::string apiKey, const std::string secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::PORTFOLIO, useInternal);
    }
}
