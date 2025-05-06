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
        event.tradeId = json_value["t"].asInt64();
        event.isInOrderBook = json_value["w"].asBool();
        event.isMaker = json_value["m"].asBool();
        event.createTime = json_value["O"].asUInt64();
        event.quoteVolume = str_to_dobule(json_value["Q"]);
        event.latestQuoteVolume = str_to_dobule(json_value["Y"]);
        event.filledQuoteVolume = str_to_dobule(json_value["Z"]);
        event.selfTradePreventionMode = json_value["V"].asString();

        return event;
    }
}