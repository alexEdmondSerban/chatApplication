// server.cpp
#include "server.hpp"

// Function to initialize Winsock
void initialize_winsock() {
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        exit(1);
    }
}

// Function to clean up Winsock
void cleanup_winsock() {
    WSACleanup();
}

// Function to handle client connections
void client_handler(SOCKET client_socket) {
    char buffer[1024];     // Buffer to store incoming messages
    while (true) {         // Continuous loop to keep receiving messages
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0); // Receive message

        if (bytes_received <= 0) {          // If no message or error, disconnect
            std::cout << "Client disconnected." << std::endl;
            closesocket(client_socket);     // Close the client socket
            return;
        }

        buffer[bytes_received] = '\0';      // Null-terminate the received string
        std::cout << "Client: " << buffer << std::endl;  // Print the message from the client

        // Echo message back to client
        send(client_socket, buffer, bytes_received, 0);
    }
}

// Function to start the server
void start_server() {
    // Initialize Winsock
    initialize_winsock();

    // Create socket for server
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Server socket creation failed: " << WSAGetLastError() << std::endl;
        cleanup_winsock();
        return;
    }

    // Bind socket to address and port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Use any available IP address of the machine
    server_addr.sin_port = htons(8080);       // Port number 8080

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        cleanup_winsock();
        return;
    }

    // Listen for incoming connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        cleanup_winsock();
        return;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    // Accept and handle incoming connections
    while (true) {
        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(server_socket);
            cleanup_winsock();
            return;
        }

        std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Create a thread to handle the client
        std::thread(client_handler, client_socket).detach();
    }

    closesocket(server_socket); // Close server socket
    cleanup_winsock();          // Clean up Winsock
}