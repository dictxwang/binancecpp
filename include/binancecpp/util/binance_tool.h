#ifndef _UTIL_BINANCE_TOOL_H_
#define _UTIL_BINANCE_TOOL_H_

#include <sys/time.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <random>
#include <iomanip>
#include "binancecpp/json/json.h"

namespace binance {

    static double str_to_dobule(Json::Value& value) {
        return std::stod(value.asString());
    }

    static std::string serialize_json_value(Json::Value& value) {
        Json::StreamWriterBuilder writer;
        std::string jsonString = Json::writeString(writer, value);
        return jsonString;
    }

    //---------------------------------
    static time_t get_current_epoch( ) {

        struct timeval tv;
        gettimeofday(&tv, NULL); 

        return tv.tv_sec ;
    }

    //---------------------------------
    static uint64_t get_current_ms_epoch( ) {

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
    
    static std::string base64_encode(const unsigned char* data, size_t len) {
        static const char* base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";
        std::string encoded;
        int val = 0, valb = -6;
        for (size_t i = 0; i < len; ++i) {
            val = (val << 8) + data[i];
            valb += 8;
            while (valb >= 0) {
                encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (encoded.size() % 4) encoded.push_back('=');
        return encoded;
    }

    /*
    static unsigned char* parse_private_key(std::string public_key, std::string private_key) {

        unsigned char pkey[crypto_sign_PUBLICKEYBYTES];
        unsigned char skey[crypto_sign_SECRETKEYBYTES];
        memcpy(pkey, public_key.c_str(), crypto_sign_PUBLICKEYBYTES);
        memcpy(skey, private_key.c_str(), crypto_sign_SECRETKEYBYTES);
        crypto_sign_keypair(pkey, skey);
        return skey;
    }

    static std::string sign_payload_by_ed25519(unsigned char* skey, std::string &payload) {
        
        unsigned char signature[crypto_sign_BYTES];
        crypto_sign_detached(signature,
                            nullptr, // Signature length (optional, not needed here)
                            (const unsigned char*)payload.c_str(),
                            payload.size(),
                            skey);

        // Encode the signature in Base64
        std::string base64_signature = base64_encode(signature, crypto_sign_BYTES);
        return base64_signature;
    }
    */

    static std::string generate_uuid() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        std::uniform_int_distribution<> dis3(8, 11); // For variant bits
    
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
    
        for (int i = 0; i < 8; ++i) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 4; ++i) ss << dis(gen);
        ss << "-4"; // UUID version 4
        for (int i = 0; i < 3; ++i) ss << dis(gen);
        ss << "-";
        ss << dis3(gen); // Variant bits
        for (int i = 0; i < 3; ++i) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 12; ++i) ss << dis(gen);
    
        return ss.str();
    }
    
    static EVP_PKEY* parse_private_key(std::string private_key) {
        // Create a BIO memory buffer from the PEM string
        BIO* bio = BIO_new_mem_buf(private_key.c_str(), -1);
        if (!bio) {
            return nullptr;
        }

        // Read the private key from the BIO
        EVP_PKEY* parsed_private_key = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
        BIO_free(bio);
        return parsed_private_key;
    }

    static std::string sign_payload_by_ed25519(EVP_PKEY* parsed_private_key, std::string &payload) {
        // Create a context for signing
        EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
        if (!md_ctx) {
            throw std::runtime_error("failed to new ctx");
        }

        // Initialize the signing operation
        if (EVP_DigestSignInit(md_ctx, NULL, NULL, NULL, parsed_private_key) <= 0) {
            throw std::runtime_error("failed to sign init");
        }

        // Sign the message
        size_t signature_len = 0;
        if (EVP_DigestSign(md_ctx, NULL, &signature_len, (const unsigned char*)payload.data(), payload.size()) <= 0) {
            throw std::runtime_error("failed to digest init");
        }
        std::vector<unsigned char> signature(signature_len);
        if (EVP_DigestSign(md_ctx, signature.data(), &signature_len, (const unsigned char*)payload.data(), payload.size()) <= 0) {
            throw std::runtime_error("failed to signature");
        }

        // Resize the signature to the actual size
        signature.resize(signature_len);

        std::string base64_signature = base64_encode(signature.data(), signature.size());
        return base64_signature;
    }


    //--------------------------------
    static void split_string( std::string &s, char delim, std::vector <std::string> &result) {

        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            result.push_back(item);
        }
        
    }


}
#endif