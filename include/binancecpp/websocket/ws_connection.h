#ifndef _WS_CONNECTION_H_
#define _WS_CONNECTION_H_

#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netdb.h>
#include <unistd.h>
#include <random>
#include <arpa/inet.h>
#include <stdexcept>

SSL* create_tls_connection(int& socket_fd, const std::string& host, int port, std::string& remote_ip, std::string& local_ip);
bool perform_websocket_handshake(SSL* ssl, const std::string& host, const std::string& path);

#endif