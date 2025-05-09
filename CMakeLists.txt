cmake_minimum_required(VERSION 3.10)

project(binancecpp VERSION 0.1.0 LANGUAGES CXX)

set(PROJECT_NAME "binancecpp" VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "Debug")

# Find Curl
# find_package(CURL REQUIRED)
# Find OpenSSL
# find_package(OpenSSL REQUIRED)
include(CMakeFindDependencyMacro)
find_dependency(CURL REQUIRED)
find_dependency(OpenSSL REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CUSTOM_LIB_DIR "/usr/lib/x86_64-linux-gnu")
    set(CUSTOM_INCLUDE_DIR "/usr/include/x86_64-linux-gnu")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CUSTOM_LIB_DIR "/usr/local/lib")
    set(CUSTOM_INCLUDE_DIR "/usr/local/include")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CUSTOM_LIB_DIR "/usr/local/lib")
    set(CUSTOM_INCLUDE_DIR "/usr/local/include")
else()
    message(WARNING "Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif()

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CUSTOM_LIB_DIR}
)

FILE(GLOB_RECURSE LIB_BINANCE_SRC RELATIVE ${CMAKE_SOURCE_DIR}
    "src/*.cpp")
add_library(binancecpp ${LIB_BINANCE_SRC})

# add_subdirectory(lib/jsoncpp-1.8.3)
# target_compile_definitions(binancecpp PUBLIC "JSONCPP")

# # Specify include directories
target_include_directories(binancecpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CUSTOM_INCLUDE_DIR}>
)
target_link_libraries(binancecpp
    PUBLIC CURL::libcurl
    PUBLIC OpenSSL::SSL OpenSSL::Crypto
)

# Set library properties
set_target_properties(binancecpp PROPERTIES
    VERSION 0.1.0
    SOVERSION 1
    INTERFACE_LINK_LIBRARIES "CURL::libcurl"
    INTERFACE_LINK_LIBRARIES "OpenSSL::SSL"
    INTERFACE_LINK_LIBRARIES "OpenSSL::Crypto"
)

# Install your library
install(TARGETS binancecpp
    EXPORT binancecppTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/binancecpp
)

# Install the export set
install(EXPORT binancecppTargets
    FILE binancecppTargets.cmake
    NAMESPACE binancecpp::
    DESTINATION lib/cmake/binancecpp
)

# Generate package configuration files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/binancecppConfigVersion.cmake"
    VERSION 0.1.0
    COMPATIBILITY AnyNewerVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/binancecppConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/binancecppConfig.cmake" @ONLY
)


install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/binancecppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/binancecppConfigVersion.cmake"
    DESTINATION lib/cmake/binancecpp
)
