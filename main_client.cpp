// main_client.cpp
#include "client.hpp"      // Include the header for client functions
#include <iostream>        // For input and output
#include <thread>          // For multithreading
#include <WinSock2.h>      // For Windows socket functions
#include <Ws2tcpip.h>      // For InetPton function
#pragma comment(lib, "Ws2_32.lib") // Link with Winsock library

int main() {
    initialize_winsock();   // Initialize Winsock

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0); // Create socket
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        cleanup_winsock();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(client_socket);
        cleanup_winsock();
        return 1;
    }

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        cleanup_winsock();
        return 1;
    }

    std::cout << "Connected to the server." << std::endl;

    std::thread(receive_messages, client_socket).detach(); // Start receive thread
    send_messages(client_socket);                          // Start sending messages

    closesocket(client_socket); // Close socket
    cleanup_winsock();          // Clean up Winsock
    return 0;
}