#ifndef _UTIL_BINANCE_TOOL_H_
#define _UTIL_BINANCE_TOOL_H_

#include <sys/time.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "binancecpp/json/json.h"

namespace binance {

    static double str_to_dobule(Json::Value& value) {
        return std::stod(value.asString());
    }

    //---------------------------------
    static time_t get_current_epoch( ) {

        struct timeval tv;
        gettimeofday(&tv, NULL); 

        return tv.tv_sec ;
    }

    //---------------------------------
    static unsigned long get_current_ms_epoch( ) {

        struct timeval tv;
        gettimeofday(&tv, NULL); 

        return tv.tv_sec * 1000 + tv.tv_usec / 1000 ;

    }

    //--------------------------------------
    static std::string b2a_hex(char *byte_arr, int n) {

        const static std::string HexCodes = "0123456789abcdef";
        std::string HexString;
        for ( int i = 0; i < n ; ++i ) {
            unsigned char BinValue = byte_arr[i];
            HexString += HexCodes[( BinValue >> 4 ) & 0x0F];
            HexString += HexCodes[BinValue & 0x0F];
        }
        return HexString;
    }

    //---------------------------
    static std::string hmac_sha256(const char *key, const char *data) {

        unsigned char* digest;
        digest = HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)data, strlen(data), NULL, NULL);    
        return b2a_hex( (char *)digest, 32 );
    }   

    //------------------------------
    static std::string sha256(const char *data ) {

        unsigned char digest[32];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, data, strlen(data) );
        SHA256_Final(digest, &sha256);
        return b2a_hex( (char *)digest, 32 );
        
    }
}
#endif