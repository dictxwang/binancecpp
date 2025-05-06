#ifndef _UTIL_COMMON_TOOL_H_
#define _UTIL_COMMON_TOOL_H_

#include "binancecpp/json/json.h"

// Define a `triple` struct
template <typename T1, typename T2, typename T3>
struct triple {
    T1 first;
    T2 second;
    T3 third;

    // Constructor
    triple(T1 f, T2 s, T3 t) : first(f), second(s), third(t) {}

    // Default Constructor
    triple() = default;
};

namespace binance {

    static double str_to_dobule(Json::Value& value) {
        return std::stod(value.asString());
    }
}

#endif