#include "binancecpp/binance_ws_model.h"

namespace binance {

    WsBookTickerEvent convertJsonToWsBookTickerEvent(Json::Value &json_value) {
        WsBookTickerEvent event;
        if (json_value.isMember("s")) {
            event.symbol = json_value["s"].asString();
        }
        if (json_value.isMember("u")) {
            event.updateId = json_value["u"].asUInt64();
        }
        if (json_value.isMember("a")) {
            event.bestAskPrice = str_to_dobule(json_value["a"]);
        }
        if (json_value.isMember("A")) {
            event.bestAskQty = str_to_dobule(json_value["A"]);
        }
        if (json_value.isMember("b")) {
            event.bestBidPrice = str_to_dobule(json_value["b"]);
        }
        if (json_value.isMember("B")) {
            event.bestBidQty = str_to_dobule(json_value["B"]);
        }
        return event;
    }

    WsSpotBalanceUpdateEvent convertJsonToWsSpotBalanceUpdateEvent(Json::Value &json_value) {
        WsSpotBalanceUpdateEvent event;
        if (json_value.isMember("a")) {
            event.asset = json_value["a"].asString();
        }
        if (json_value.isMember("d")) {
            event.change = str_to_dobule(json_value["d"]);
        }
        if (json_value.isMember("T")) {
            event.transactionTime = json_value["T"].asUInt64();
        }
        return event;
    }

    WsAccountUpdateBalanceEvent convertJsonToWsAccountUpdateBalanceEvent(Json::Value &json_value) {
        WsAccountUpdateBalanceEvent event;
        if (json_value.isMember("a")) {
            event.asset = json_value["a"].asString();
        }
        if (json_value.isMember("f")) {
            event.free = str_to_dobule(json_value["f"]);
        }
        if (json_value.isMember("l")) {
            event.locked = str_to_dobule(json_value["l"]);
        }
        return event;
    }

    WsSpotAccountUpdateEvent convertJsonToWsWsSpotAccountUpdateEvent(Json::Value &json_value) {
        WsSpotAccountUpdateEvent event;
        if (json_value.isMember("u")) {
            event.updateTime = json_value["u"].asUInt64();
        }
        if (json_value.isMember("B")) {
            Json::Value balances = json_value["B"];
            for (int i = 0; i < balances.size(); i++) {
                event.balances.push_back(convertJsonToWsAccountUpdateBalanceEvent(balances[i]));
            }
        }
        return event;
    }

    WsSpotOrderUpdateEvent convertJsonToWsSpotOrderUpdateEvent(Json::Value &json_value) {
        WsSpotOrderUpdateEvent event;
        event.symbol = json_value["s"].asString();
        event.clientOrderId = json_value["c"].asString();
        event.side = json_value["S"].asString();
        event.type = json_value["o"].asString();
        event.timeInForce = json_value["f"].asString();
        event.volume = str_to_dobule(json_value["q"]);
        event.price = str_to_dobule(json_value["p"]);
        event.stopPrice = str_to_dobule(json_value["P"]);
        event.iceBergVolume = str_to_dobule(json_value["F"]);
        event.orderListId = json_value["g"].asInt64();
        event.origCustomOrderId = json_value["C"].asString();
        event.executionType = json_value["x"].asString();
        event.status = json_value["X"].asString();
        event.rejectReason = json_value["r"].asString();

        event.id = json_value["i"].asUInt64();
        event.latestVolume = str_to_dobule(json_value["l"]);
        event.filledVolume = str_to_dobule(json_value["z"]);
        event.latestPrice = str_to_dobule(json_value["L"]);
        if (json_value["N"].isNull()) {
            event.feeAsset = "";
        } else {
            event.feeAsset = json_value["N"].asString();
        }
        event.feeCost = str_to_dobule(json_value["n"]);
        event.transactionTime = json_value["T"].asUInt64();
        event.tradeId = json_value["t"].asUInt64();
        event.isInOrderBook = json_value["w"].asBool();
        event.isMaker = json_value["m"].asBool();
        event.createTime = json_value["O"].asUInt64();
        event.quoteVolume = str_to_dobule(json_value["Q"]);
        event.latestQuoteVolume = str_to_dobule(json_value["Y"]);
        event.filledQuoteVolume = str_to_dobule(json_value["Z"]);
        event.selfTradePreventionMode = json_value["V"].asString();

        return event;
    }

    WsFuturesBookTickerEvent convertJsonToWsFuturesBookTickerEvent(Json::Value &json_value) {
        WsFuturesBookTickerEvent event;
        if (json_value.isMember("s")) {
            event.symbol = json_value["s"].asString();
        }
        if (json_value.isMember("E")) {
            event.eventTime = json_value["E"].asUInt64();
        }
        if (json_value.isMember("T")) {
            event.transactionTime = json_value["T"].asUInt64();
        }
        if (json_value.isMember("u")) {
            event.updateId = json_value["u"].asUInt64();
        }
        if (json_value.isMember("a")) {
            event.bestAskPrice = str_to_dobule(json_value["a"]);
        }
        if (json_value.isMember("A")) {
            event.bestAskQty = str_to_dobule(json_value["A"]);
        }
        if (json_value.isMember("b")) {
            event.bestBidPrice = str_to_dobule(json_value["b"]);
        }
        if (json_value.isMember("B")) {
            event.bestBidQty = str_to_dobule(json_value["B"]);
        }
        return event;
    }

    WsFuturesMarkPriceEvent convertJsonToWsFuturesMarkPriceEvent(Json::Value &json_value) {
        WsFuturesMarkPriceEvent event;
        if (json_value.isMember("E")) {
            event.eventTime = json_value["E"].asUInt64();
        }
        if (json_value.isMember("s")) {
            event.symbol = json_value["s"].asString();
        }
        if (json_value.isMember("p")) {
            event.markPrice = str_to_dobule(json_value["p"]);
        }
        if (json_value.isMember("i")) {
            event.indexPrice = str_to_dobule(json_value["i"]);
        }
        if (json_value.isMember("P")) {
            event.estimatedSettlePrice = str_to_dobule(json_value["P"]);
        }
        if (json_value.isMember("r")) {
            event.fundingRate = str_to_dobule(json_value["r"]);
        }
        if (json_value.isMember("T")) {
            event.nextFundingTime = json_value["T"].asUInt64();
        }
        return event;
    }

    WsFuturesAccountUpdateEvent convertJsonToWsFuturesAccountUpdateEvent(Json::Value &json_value) {
        WsFuturesAccountUpdateEvent event;
        event.eventType = json_value["e"].asString();
        event.eventTIme = json_value["E"].asUInt64();
        event.transactionTime = json_value["T"].asUInt64();
        if (json_value.isMember("a")) {
            event.eventReasonType = json_value["a"]["m"].asString();
            if (json_value["a"].isMember("B")) {
                Json::Value balances = json_value["a"]["B"];
                for (int i = 0; i < balances.size(); i++) {
                    WsFuturesAccountUpdateBalanceEvent be;
                    be.asset = balances[i]["a"].asString();
                    be.walletBalance = str_to_dobule(balances[i]["wb"]);
                    be.crossWalletBalance = str_to_dobule(balances[i]["cw"]);
                    be.balanceChange = str_to_dobule(balances[i]["bc"]);
                    event.balances.push_back(be);
                }
            }
            if (json_value["a"].isMember("P")) {
                Json::Value positions = json_value["a"]["P"];
                for (int i = 0; i < positions.size(); i++) {
                    WsFuturesAccountUpdatePositionEvent pe;
                    pe.symbol = positions[i]["s"].asString();
                    pe.postionAmout = str_to_dobule(positions[i]["pa"]);
                    pe.entryPrice = str_to_dobule(positions[i]["ep"]);
                    pe.breakevenPrice = str_to_dobule(positions[i]["bep"]);
                    pe.accumulatedRealized = str_to_dobule(positions[i]["cr"]);
                    pe.unrealizedPnL = str_to_dobule(positions[i]["up"]);
                    pe.marginType = positions[i]["mt"].asString();
                    pe.isolatedWallet = str_to_dobule(positions[i]["iw"]);
                    pe.positionSide = positions[i]["ps"].asString();
                    event.positions.push_back(pe);
                }
            }
        }
        return event;
    }
    WsFuturesOrderTradeUpdateEvent convertJsonToWsFuturesOrderTradeUpdateEvent(Json::Value &json_value) {
        WsFuturesOrderTradeUpdateEvent event;
        event.eventType = json_value["e"].asString();
        event.eventTIme = json_value["E"].asUInt64();
        event.transactionTime = json_value["T"].asUInt64();

        if (json_value.isMember("o")) {
            Json::Value order = json_value["o"];
            event.symbol = order["s"].asString();
            event.clientOrderId = order["c"].asString();
            event.side = order["S"].asString();
            event.type = order["o"].asString();
            event.timeInForce = order["f"].asString();
            event.volume = str_to_dobule(order["q"]);
            event.price = str_to_dobule(order["p"]);
            event.averagePrice = str_to_dobule(order["ap"]);
            event.stopPrice = str_to_dobule(order["sp"]);
            event.executionType = order["x"].asString();
            event.orderStatus = order["X"].asString();
            event.id = order["i"].asUInt64();
            event.latestVolume = str_to_dobule(order["l"]);
            event.filledVolume = str_to_dobule(order["z"]);
            event.latestPrice = str_to_dobule(order["L"]);
            event.feeAsset = order["N"].asString();
            event.feeCost = str_to_dobule(order["n"]);
            event.tradeId = order["t"].asUInt64();
            event.bidNotional = str_to_dobule(order["b"]);
            event.askNotional = str_to_dobule(order["a"]);
            event.isMaker = order["m"].asBool();
            event.isReduce = order["R"].asBool();
            event.workingType = order["wt"].asString();
            event.originalOrderType = order["ot"].asString();
            event.positionSide = order["ps"].asString();
            event.isClosingPosition = order["cp"].asBool();
            if (order.isMember("AP")) {
                event.activationPrice = str_to_dobule(order["AP"]);
            }
            if (order.isMember("cr")) {
                event.callbackRate = str_to_dobule(order["cr"]);
            }
            event.priceProtect = order["pP"].asBool();
            event.realizedPnL = str_to_dobule(order["rp"]);
            event.stp = order["V"].asString();
            event.priceMode = order["pm"].asString();
            event.gtd = str_to_dobule(order["gtd"]);
        }
        return event;
    }
    WsFuturesOrderCallbackEvent convertJsonToWsFuturesOrderCallbackEvent(Json::Value &json_value) {
        WsFuturesOrderCallbackEvent event;
        event.id = json_value["id"].asString();
        event.status = json_value["status"].asInt();
        if (event.status != WsCallbackStatusOK) {
            if (json_value.isMember("error")) {
                WsFuturesCallbackError error;
                error.code = json_value["error"]["code"].asInt();
                error.msg = json_value["error"]["msg"].asString();
                event.error = error;
            }
        } else {
            if (json_value.isMember("result")) {
                Json::Value order = json_value["result"];
                WsFuturesOrderCallbackEventResult result;
                result.orderId = order["orderId"].asUInt64();
                result.symbol = order["symbol"].asString();
                result.status = order["status"].asString();
                result.clientOrderId = order["clientOrderId"].asString();
                result.price = str_to_dobule(order["price"]);
                result.avgPrice = str_to_dobule(order["avgPrice"]);
                result.origQty = str_to_dobule(order["origQty"]);
                result.executedQty = str_to_dobule(order["executedQty"]);
                result.cumQty = str_to_dobule(order["cumQty"]);
                result.cumQuote = str_to_dobule(order["cumQuote"]);
                result.timeInForce = order["timeInForce"].asString();
                result.type = order["type"].asString();
                result.reduceOnly = order["reduceOnly"].asBool();
                result.closePosition = order["closePosition"].asBool();
                result.side = order["side"].asString();
                result.positionSide = order["positionSide"].asString();
                result.stopPrice = str_to_dobule(order["stopPrice"]);
                result.workingType = order["workingType"].asString();
                result.priceProtect = order["priceProtect"].asBool();
                result.origType = order["origType"].asString();
                result.priceMatch = order["priceMatch"].asString();
                result.selfTradePreventionMode = order["selfTradePreventionMode"].asString();
                result.goodTillDate = order["goodTillDate"].asUInt64();
                result.updateTime = order["updateTime"].asUInt64();
                event.result = result;
            }
        }
        return event;
    }
}