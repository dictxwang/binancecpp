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

    void BinanceSpotRestClient::getExchangeInfo() {
        Json::Reader reader;
        std::string url = baseUrl + "/api/v3/exchangeInfo";
        // Initialize libcurl
        curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* curl = curl_easy_init();
    
        // The response data will be stored here
        std::string responseData;
        CURLcode res;
    
        // Set the URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
        // Set the callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    
        // Pass the string to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    
        // Enable SSL/TLS verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // Verify the server's certificate
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // Verify the hostname
    
    
        // Perform the request
        res = curl_easy_perform(curl);
    
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Response Data: " << responseData << std::endl;
        }
    
    }
} // namespace binance
