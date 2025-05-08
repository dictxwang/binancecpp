#include "binancecpp/binance.h"

namespace binance {
    
    void BinanceRestClient::setLocalIP(const std::string& localIP) {
        this->serverMeta.localIP= localIP;
    }
    void BinanceRestClient::setRemoteIP(const std::string& remoteIP) {
        this->serverMeta.remoteIP = remoteIP;
    }

    void BinanceRestClient::init(const std::string& apiKey, const std::string& secretKey, MarketType marketType, bool useInternal) {
        this->apiKey = apiKey;
        this->secretKey = secretKey;
        this->marketType = marketType;
        this->serverMeta.useInternal = useInternal;
        std::pair<std::string, std::string> baseUrlPair = getBaseUrl(marketType, useInternal);
        this->serverMeta.baseUrl = baseUrlPair.first;
        this->serverMeta.serverHost = baseUrlPair.second;;
        this->serverMeta.serverPort = "443";
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
            if (!this->serverMeta.localIP.empty()) {
                curl_easy_setopt(curl, CURLOPT_INTERFACE, serverMeta.localIP.c_str());
            }

            if (!this->serverMeta.remoteIP.empty()) {
                struct curl_slist* resolve_list = nullptr;
                // curl_easy_setopt(curl, CURLOPT_CONNECT_TO, (host + ":" + port + "::" + remoteIP).c_str());
                std::string resolve_entry = this->serverMeta.serverHost + ":" + this->serverMeta.serverPort + ":" + serverMeta.remoteIP;
                resolve_list = curl_slist_append(resolve_list, resolve_entry.c_str());
                curl_easy_setopt(curl, CURLOPT_RESOLVE, resolve_list);
            }


            if ( extra_http_header.size() > 0 ) {
                
                struct curl_slist *chunk = nullptr;
                for ( size_t i = 0; i < extra_http_header.size(); ++i ) {
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

    void BinanceRestClient::api_action(std::string &api_url, binance::ApiSecretType sec_type, std::string &method, std::vector <std::string> &header, std::vector <std::string> &query_params, std::vector <std::string> &form_params, binance::CommonRestResponse<std::string> &action_response) {
        std::string queryString;
        std::string formString;
        if (sec_type == binance::ApiSecretType::SecTypeSignature) {
            // child instance give timestamp
            // query_params.push_back("timestamp=");
        }

        CURL* curl = curl_easy_init();
        CURLcode call_code;
        std::string call_result;

        char* delim = "=";

        if (!query_params.empty()) {
            std::sort(query_params.begin(), query_params.end());
            std::vector<std::string> sorted_params;
            for (int i = 0; i < query_params.size(); ++i) {
                std::vector<std::string> pair;
                strHelper::splitStr(pair, query_params[i], delim);
                if (pair.size() == 2) {
                    char* escape_value = curl_easy_escape(curl, pair[1].c_str(), 0);
                    sorted_params.push_back(pair[0] + "=" + std::string(escape_value));
                    curl_free(escape_value);
                }
            }
            queryString = strHelper::joinStrings(sorted_params, "&");
        }

        if (!form_params.empty()) {
            std::sort(form_params.begin(), form_params.end());
            std::vector<std::string> sorted_params;
            for (int i = 0; i < query_params.size(); ++i) {
                std::vector<std::string> pair;
                strHelper::splitStr(pair, query_params[i], delim);
                if (pair.size() == 2) {
                    char* escape_value = curl_easy_escape(curl, pair[1].c_str(), 0);
                    sorted_params.push_back(pair[0] + "=" + std::string(escape_value));
                    curl_free(escape_value);
                }
            }
            formString = strHelper::joinStrings(sorted_params, "&");
        }

        if (formString.size() != 0) {
            header.push_back("Content-Type: application/x-www-form-urlencoded");
        }

        if (sec_type == binance::SecTypeApiKey || sec_type == binance::SecTypeSignature) {
            header.push_back("X-MBX-APIKEY: " + this->apiKey);
        }

        if (sec_type == binance::SecTypeSignature) {
            std::string payload = queryString + formString;
            std::string signature = hmac_sha256(this->secretKey.c_str() , payload.c_str());
            if (queryString.size() == 0) {
                queryString.append("signature=").append(signature);
            } else {
                queryString.append("&signature=").append(signature);
            }
        }

        std::string fullUrl = api_url;
        if (queryString.size() > 0) {
            fullUrl.append("?").append(queryString);
        }

        try {
            // Set default server-meta
            binance::RestServerMeta serverMeta = binance::RestServerMeta{};
            call_code = curl_api_with_header(curl, fullUrl, call_result, header, formString, method);
        } catch (std::exception &e) {
            action_response.code = -500;
            action_response.msg = "parse json error: " + std::string(e.what());
            if (curl != nullptr) {
                curl_easy_cleanup(curl);
            }
            return;
        }

        if (call_result.size() > 0 ) {
            // std::cout << "api response: " << call_result << std::endl;
            if (call_result.find("\"code\"") && call_result.find("\"msg\"")) {
                try {
                    Json::Value json_result;
                    Json::Reader reader;
                    json_result.clear();
                    reader.parse(call_result , json_result);
    
                    if (parse_api_has_error(json_result, action_response)) {
                        if (curl != nullptr) {
                            curl_easy_cleanup(curl);
                        }
                        return;
                    }

                } catch (std::exception &e) {
                    action_response.code = -900;
                    action_response.msg = "parse json error: " + std::string(e.what());
                }
            }
            // 透传给调用方解析
            action_response.data = call_result;
        } else {
            // notthing to parse
            action_response.code = -100;
            action_response.msg = "no response content";
        }

        if (curl != nullptr) {
            curl_easy_cleanup(curl);
        }
    }

    void BinanceRestClient::api_action_get_no_sec(std::string &api_url, std::vector <std::string> &query_params, binance::CommonRestResponse<std::string> &action_response) {
        std::vector <std::string> empty;
        std::string method = "GET";
        api_action(api_url, ApiSecretType::SecTypeNone, method, empty, query_params, empty, action_response);
    }
}