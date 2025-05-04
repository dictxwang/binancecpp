#include "binancecpp/binance_spot.h"
#include "binancecpp/json/json.h"

namespace binance {

    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
        size_t totalSize = size * nmemb;
        userData->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    void BinanceSpotRestClient::init(const std::string& apiKey, const std::string& secretKey, bool useInternal) {
        BinanceRestClient::init(apiKey, secretKey, MarketType::SPOT, useInternal);
    }

    void BinanceSpotRestClient::get_exchangeInfo(binance::CommonRestResponse<binance::ExchangeInfoResponse> &response) {
        Json::Reader reader;
        std::string url = baseUrl + "/api/v3/exchangeInfo";
        // TODO Initialize libcurl
        // curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();
    
        CURLcode res;
        std::string str_result;
        res = curl_api(curl, url, str_result);

        if (str_result.size() > 0 ) {
            try {
                Json::Value json_result;
                Json::Reader reader;
                json_result.clear();	
                reader.parse(str_result , json_result);
                
                // parse json to 
                if (json_result.isMember("symbols")) {
                    Json::Value symbols = json_result["symbols"];
                    for (int i = 0; i < symbols.size(); i++) {
                        ExchangeInfo exchangeInfo;
                        exchangeInfo.symbol = symbols[i]["symbol"].asString();
                        exchangeInfo.status = symbols[i]["status"].asString();
                        exchangeInfo.baseAsset = symbols[i]["baseAsset"].asString();
                        exchangeInfo.quoteAsset = symbols[i]["quoteAsset"].asString();
                        exchangeInfo.minPrice = symbols[i]["filters"][0]["minPrice"].asDouble();
                        exchangeInfo.maxPrice = symbols[i]["filters"][0]["maxPrice"].asDouble();
                        exchangeInfo.tickSize = symbols[i]["filters"][0]["tickSize"].asDouble();
                        exchangeInfo.stepSize = symbols[i]["filters"][0]["stepSize"].asDouble();
                        response.data.symbols.push_back(exchangeInfo);
                    }
                }
            } catch (std::exception &e ) {
                throw e;
            }
        } else {
            // notthing to parse
        }

        if (curl != nullptr) {
            curl_easy_cleanup(curl);
        }
    }
} // namespace binance
