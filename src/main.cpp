#include "include/binance_spot.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    // Create an instance of BinanceSpotRestClient
    binance::BinanceSpotRestClient binanceSpot;

    binanceSpot.init("", "");

    binanceSpot.getExchangeInfo();
}
