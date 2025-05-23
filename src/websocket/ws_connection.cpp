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
    
    // Step 5: Connect to the server with timeout
    // Set the socket to non-blocking mode
    int origin_flags = fcntl(socket_fd, F_GETFL, 0);
    if (origin_flags < 0) {
        close(socket_fd);
        throw std::runtime_error("fail to get socket flag");
    }

    if (fcntl(socket_fd, F_SETFL, origin_flags | O_NONBLOCK) < 0) {
        throw std::runtime_error("fail to set socket to non-blocking mode");
    }

    int connect_result = connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (connect_result == 0) {
        // Connection succeeded immediately
        // Restore socket flags
        fcntl(socket_fd, F_SETFL, origin_flags);
    } else {
        if (errno != EINPROGRESS) {
            // Connection attempt failed immediately
            throw std::runtime_error("connect fail: " + std::string(strerror(errno)));
        }

        // Use select() to wait for the socket to become writable
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(socket_fd, &writefds);
        struct timeval connect_timeout;
        connect_timeout.tv_sec = 3;
        connect_timeout.tv_usec = 0;

        connect_result = select(socket_fd + 1, nullptr, &writefds, nullptr, &connect_timeout);
        if (connect_result <= 0) {
            // Timeout or error
            if (connect_result == 0) {
                throw std::runtime_error("connect time out");
            } else {
                throw std::runtime_error("select error: " + std::string(strerror(errno)));
            }
        }

        // Check for errors on the socket
        int sock_error;
        socklen_t len = sizeof(sock_error);
        if (getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &sock_error, &len) < 0) {
            throw std::runtime_error("get sockopt failed: " + std::string(strerror(errno)));
        }

        if (sock_error != 0) {
            throw std::runtime_error("connect failed: " + std::string(strerror(errno)));
        }

        // Connection was successful
        fcntl(socket_fd, F_SETFL, origin_flags); // Restore socket flags
    }

    // // Step 5: Connect to the server
    // if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    //     close(socket_fd);
    //     throw std::runtime_error("failed to connect to server");
    // }

    // Set a timeout for receiving data
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds
    timeout.tv_usec = 0; // 0 microseconds

    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        close(socket_fd);
        throw std::runtime_error("failed to set read timeout");
    }

    // Set a timeout for sending data (optional)
    if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        close(socket_fd);
        throw std::runtime_error("failed to set write timeout");
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