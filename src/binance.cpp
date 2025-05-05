#include "binancecpp/binance.h"

namespace binance {
    
    void BinanceRestClient::setLocalIP(const std::string& localIP) {
        this->localIP = localIP;
    }
    void BinanceRestClient::setRemoteIP(const std::string& remoteIP) {
        this->remoteIP = remoteIP;
    }
    void BinanceRestClient::init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal) {
        this->apiKey = apiKey;
        this->secretKey = secretKey;
        this->marketType = marketType;
        this->useInternal = useInternal;
        std::pair<std::string, std::string> baseUrlPair = getBaseUrl(marketType, useInternal);
        this->baseUrl = baseUrlPair.first;
        this->serverHost = baseUrlPair.second;
        this->serverPort = "443";
    }

    CURLcode BinanceRestClient::curl_api(CURL* curl, std::string &url, std::string &result_json ) {
        std::vector <std::string> v;
        std::string action = "GET";
        std::string post_data = "";
        return curl_api_with_header(curl, url , result_json , v, post_data , action );	
    } 

    CURLcode BinanceRestClient::curl_api_with_header(CURL *curl, std::string &url, std::string &str_result, std::vector <std::string> &extra_http_header , std::string &post_data , std::string &action ) {

        CURLcode res = CURLE_OK;

        if(curl != nullptr) {

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_result );
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            // curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");

            // Bind the request to a specific local IP or network interface
            if (!localIP.empty()) {
                curl_easy_setopt(curl, CURLOPT_INTERFACE, localIP.c_str());
            }

            if (!remoteIP.empty()) {
                struct curl_slist* resolve_list = nullptr;
                // curl_easy_setopt(curl, CURLOPT_CONNECT_TO, (host + ":" + port + "::" + remoteIP).c_str());
                std::string resolve_entry = serverHost + ":" + serverPort + ":" + remoteIP;
                resolve_list = curl_slist_append(resolve_list, resolve_entry.c_str());
                curl_easy_setopt(curl, CURLOPT_RESOLVE, resolve_list);
            }


            if ( extra_http_header.size() > 0 ) {
                
                struct curl_slist *chunk = nullptr;
                for ( int i = 0 ; i < extra_http_header.size() ;i++ ) {
                    chunk = curl_slist_append(chunk, extra_http_header[i].c_str() );
                }
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
            }

            if ( post_data.size() > 0 || action == "POST" || action == "PUT" || action == "DELETE" ) {

                if ( action == "PUT" || action == "DELETE" ) {
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, action.c_str() );
                }
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str() );
            }

            // TODO Enable SSL/TLS verification
            // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // Verify the server's certificate
            // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // Verify the hostname

            res = curl_easy_perform(curl);
        }

        return res;
    }
}