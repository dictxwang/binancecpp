cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "cpp_binance")
set(PROCESS_MAIN "process_main")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
# set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    include
    src
    lib/jsoncpp-1.8.3/include
)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
# Find Curl
find_package(CURL REQUIRED)

# message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
# message(STATUS "CMAKE_CURRENT_LIB_DIR: ${CMAKE_CURRENT_LIB_DIR}")

# config
FILE(GLOB_RECURSE LIB_BINANCE_SRC RELATIVE ${CMAKE_SOURCE_DIR}
    "src/binance*.cpp")
add_library(binance ${LIB_BINANCE_SRC})

add_executable(${PROCESS_MAIN} "src/main.cpp")
# Include headers
target_include_directories(${PROCESS_MAIN} PRIVATE ${CURL_INCLUDE_DIR})
target_include_directories(${PROCESS_MAIN} PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROCESS_MAIN}
    PRIVATE OpenSSL::SSL OpenSSL::Crypto
    PRIVATE CURL::libcurl
    binance
)