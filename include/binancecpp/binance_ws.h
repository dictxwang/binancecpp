#ifndef _BINANCE_WS_H_
#define _BINANCE_WS_H_

#include <string>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include "binance.h"
#include "util/common_tool.h"
#include "websocket/ws_connection.h"
#include "websocket/ws_packet.h"
#include "moodycamel/concurrentqueue.h"

using namespace std;

namespace binance {

    const int WEBSOCKET_HEADER_MIN_SIZE = 5;

    const triple<string, int, string> SPOT_WS_ENDPOINT = triple(string("stream.binance.com"), 9443, string("/ws"));
    const triple<string, int, string> SPOT_COMBINE_WS_ENDPOINT = triple(string("stream.binance.com"), 9443, string("/stream?streams="));
    const triple<string, int, string> SPOT_TRADING_WS_ENDPOINT = triple(string("ws-api.binance.com"), 443, string("/ws-api/v3?returnRateLimits=false"));

    
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
        BinanceWsClient() {
            this->ssl = nullptr;
            this->customCallback = nullptr;
            this->messageChannel = nullptr;
        }

        // Destructor
        virtual ~BinanceWsClient() {
            release_resource();
            if (this->parsedSecretKey != nullptr) {
                delete(this->parsedSecretKey);
            }
            if (this->messageChannel != nullptr) {
                string temp;
                while (this->messageChannel->try_dequeue(temp)) {}
            }
        }

        // Public methods
    
    protected:
        std::string apiKey;
        std::string secretKey;
        EVP_PKEY* parsedSecretKey;
        std::string localIP;
        std::string remoteIP;
        triple<string, int, string> wsEndpoint;
        MarketType marketType;
        bool useInternal;
        bool useCombine;
        bool useTrading;

        int socketFd;
        SSL* ssl;

        // std::mutex mutex;
        // std::unique_lock<std::mutex> lock;
        shared_mutex rw_lock;

        ByteBuffer recvBuffer;
        ByteBuffer msgBuffer;
        WS_CB customCallback;
        shared_ptr<moodycamel::ConcurrentQueue<std::string>> messageChannel;
        std::string sessionID;

        bool isConnected;
        bool isLogoned;

    public:
        // Public methods
        void setLocalIP(const std::string localIP); // call before init
        void setRemoteIP(const std::string remoteIP); // call before init
        void setMessageChannel(shared_ptr<moodycamel::ConcurrentQueue<std::string>> messageChannel);
        void setMessageCallback(WS_CB customCallback);
        void stop(); // close connection and stop reading loop
        void free(); // free resources

    protected:
        // Protected methods
        void init(const std::string apiKey, const std::string secretKey, MarketType marketType, bool useInternal, bool useCombine, bool useTrading);
        // virtual std::string make_subscribe_frame(std::vector<std::string> &params);
        std::pair<bool, string> connect_endpoint(std::string& handshakePath);
        std::pair<bool, string> send_session_logon();
        std::pair<bool, string> send_subscribe(std::string& payload);
		std::pair<bool, string> start_event_loop();
        std::pair<bool, string> process_one_message(WebSocketPacket& packet, ByteBuffer& mssageBuffer);
        void release_resource();
    };
}
#endif