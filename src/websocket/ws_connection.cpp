#include "binancecpp/websocket/ws_connection.h"

SSL* create_tls_connection(int& socket_fd, const std::string& host, int port, std::string& remote_ip, std::string& local_ip) {
    // Step 1: Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();

    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        throw std::runtime_error("fail to create ssl ctx");
    }

    // Step 2: Resolve the server address
    struct hostent* server = gethostbyname(host.c_str());
    if (!server) {
        throw std::runtime_error("no such host");
    }
    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (remote_ip != "") {
        // Use the provided server address
        server_addr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
    } else {
        // Use the resolved server address
        std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    }

    // Step 3: Create a TCP socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error("fail to create socket");
    }

    // Step 4: Set the local IP address (optional)
    if (local_ip != "") {
        struct sockaddr_in local_addr{};

        // std::memset(&local_addr, 0, sizeof(local_addr)); // Clear the structure, setting all fields to zero
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(0); // Use any available port
        local_addr.sin_addr.s_addr = inet_addr(local_ip.c_str());
        if (bind(socket_fd, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
            close(socket_fd);
            throw std::runtime_error("fail to bind local ip");
        }
    }

    // Step 5: Connect to the server
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(socket_fd);
        throw std::runtime_error("failed to connect to server");
    }

    // Step 6: Create an SSL object and attach it to the socket
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket_fd);

    // Step 7: Perform the TLS connection
    if (SSL_connect(ssl) <= 0) {
        // ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(socket_fd);
        return nullptr;
    }

    return ssl;
}

bool perform_websocket_handshake(SSL* ssl, const std::string& host, const std::string& path) {
    // Step 1: Create WebSocket HTTP handshake request
    std::string websocket_key = "dGhlIHNhbXBsZSBub25jZQ=="; // Base64-encoded random key
    std::string request =
        "GET " + path + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: " + websocket_key + "\r\n"
        "Sec-WebSocket-Version: 13\r\n\r\n";

    // Step 2: Send the handshake request
    if (SSL_write(ssl, request.c_str(), request.size()) <= 0) {
        throw std::runtime_error("fail to send websocket handshake request");
    }

    // Step 3: Read the server's response
    char response[4096];
    int bytes_read = SSL_read(ssl, response, sizeof(response) - 1);
    if (bytes_read <= 0) {
        throw std::runtime_error("fail to read websocket handshake response");
    }

    response[bytes_read] = '\0';

    // Step 4: Check for "101 Switching Protocols" in the response
    if (std::string(response).find("101 Switching Protocols") == std::string::npos) {
        return false;
    }

    return true;
}