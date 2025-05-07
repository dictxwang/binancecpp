#ifndef _BINANCE_WS_H_
#define _BINANCE_WS_H_

#include <string>
#include "binance.h"
#include "util/common_tool.h"
#include "websocket/ws_connection.h"
#include "websocket/ws_packet.h"

using namespace std;

namespace binance {

    const int WEBSOCKET_HEADER_MIN_SIZE = 5;

    const triple<string, int, string> SPOT_WS_ENDPOINT = triple(string("stream.binance.com"), 9443, string("/ws"));
    const triple<string, int, string> SPOT_COMBINE_WS_ENDPOINT = triple(string("stream.binance.com"), 9443, string("/stream?streams="));
    const triple<string, int, string> SPOT_TRADING_WS_ENDPOINT = triple(string("ws-api.binance.com"), 443, string("/v3?returnRateLimits=false"));

    
    const triple<string, int, string> DELIVERY_WS_ENDPOINT = triple(string("dstream.binance.com"), 9443, string("/ws"));
    const triple<string, int, string> DELIVERY_COMBINE_WS_ENDPOINT = triple(string("dstream.binance.com"), 443, string("/stream?streams="));
    const triple<string, int, string> DELIVERY_INTERNAL_WS_ENDPOINT = triple(string("dstream-mm.binance.com"), 443, string("/ws"));
    const triple<string, int, string> DELIVERY_INTERNAL_COMBINE_WS_ENDPOINT = triple(string("dstream-mm.binance.com"), 443, string("/stream?streams="));

    const triple<string, int, string> FUTURES_WS_ENDPOINT = triple(string("fstream.binance.com"), 443, string("/ws"));
    const triple<string, int, string> FUTURES_COMBINE_WS_ENDPOINT = triple(string("fstream.binance.com"), 443, string("/stream?streams="));
    const triple<string, int, string> FUTURES_TRADING_WS_ENDPOINT = triple(string("ws-fapi.binance.com"), 443, string("/ws-fapi/v1?returnRateLimits=false"));
    const triple<string, int, string> FUTURES_INTERNAL_WS_ENDPOINT = triple(string("fstream-mm.binance.com"), 443, string("/ws"));
    const triple<string, int, string> FUTURES_INTERNAL_COMBINE_WS_ENDPOINT = triple(string("fstream-mm.binance.com"), 443, string("/stream?streams="));
    const triple<string, int, string> FUTURES_INTERNAL_TRADING_WS_ENDPOINT = triple(string("ws-fapi-mm.binance.com"), 443, string("/ws-fapi/v1?returnRateLimits=false"));

    const triple<string, int, string> OPTIONS_WS_ENDPOINT = triple(string(""), 0, string("")); // not available
    const triple<string, int, string> PORTFOLIO_WS_ENDPOINT = triple(string("fstream.binance.com"), 443, string("/pm/ws"));

    static triple<string, int, string> getWsBaseEndpoint(MarketType marketType, bool useInternal) {
        switch (marketType) {
            case SPOT:
                return SPOT_WS_ENDPOINT;
            case FUTURES:
                if (useInternal) {
                    return FUTURES_INTERNAL_WS_ENDPOINT;
                } else {
                    return FUTURES_WS_ENDPOINT;
                }
            case OPTIONS:
                return OPTIONS_WS_ENDPOINT;
            case DELIVERY:
                if (useInternal) {
                    return DELIVERY_INTERNAL_WS_ENDPOINT;
                } else {
                    return DELIVERY_WS_ENDPOINT;
                }
            case PORTFOLIO:
                return PORTFOLIO_WS_ENDPOINT;
            default:
                return triple(string(""), 0, string(""));
        }
    }

    static triple<string, int, string> getWsCombineBaseEndpoint(MarketType marketType, bool useInternal) {
        switch (marketType) {
            case SPOT:
                return SPOT_COMBINE_WS_ENDPOINT;
            case FUTURES:
                if (useInternal) {
                    return FUTURES_INTERNAL_COMBINE_WS_ENDPOINT;
                } else {
                    return FUTURES_COMBINE_WS_ENDPOINT;
                }
            case OPTIONS:
                return OPTIONS_WS_ENDPOINT;
            case DELIVERY:
                if (useInternal) {
                    return DELIVERY_INTERNAL_COMBINE_WS_ENDPOINT;
                } else {
                    return DELIVERY_COMBINE_WS_ENDPOINT;
                }
            case PORTFOLIO:
                return PORTFOLIO_WS_ENDPOINT;
            default:
                return triple(string(""), 0, string(""));
        }
    }

    static triple<string, int, string> getWsTradingEndpoint(MarketType marketType, bool useInternal) {
        switch (marketType) {
            case SPOT:
                return SPOT_TRADING_WS_ENDPOINT;
            case FUTURES:
                if (useInternal) {
                    return FUTURES_INTERNAL_TRADING_WS_ENDPOINT;
                } else {
                    return FUTURES_TRADING_WS_ENDPOINT;
                }
            default:
                return triple(string(""), 0, string(""));
        }
    }

    typedef bool (*WS_CB)(std::string& messageJson);

    class BinanceWsClient {
    public:
        // Constructor
        BinanceWsClient() {}

        // Destructor
        virtual ~BinanceWsClient() {
            release_resource();
        }

        // Public methods
    
    protected:
        std::string apiKey;
        std::string secretKey;
        unsigned char* signedSecretKey;
        std::string localIP;
        std::string remoteIP;
        triple<string, int, string> wsEndpoint;
        MarketType marketType;
        bool useInternal;

        int socketFd;
        SSL* ssl;

        ByteBuffer recvBuffer;
        ByteBuffer msgBuffer;
        WS_CB customCallback;
        std::string sessionID;

    public:
        // Public methods
        void setLocalIP(const std::string& localIP); // call before init
        void setRemoteIP(const std::string& remoteIP); // call before init

    protected:
        // Protected methods
        void init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal, bool useCombine, bool useTrading);
        // virtual std::string make_subscribe_frame(std::vector<std::string> &params);
        bool connect_endpoint(std::string& handshakePath);
        bool send_session_logon();
        bool send_subscribe(std::string& payload);
		bool start_event_loop(WS_CB customCallback);
        bool process_one_message(WebSocketPacket& packet, ByteBuffer& mssageBuffer);
        void release_resource();
    };
}
#endif