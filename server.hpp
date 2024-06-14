#ifndef SERVER_HPP
#define SERVER_HPP

#include <WinSock2.h>      // For Windows socket functions
#include <iostream>        // For input and output
#include <thread>          // For multithreading

#pragma comment(lib, "Ws2_32.lib") // Link with Winsock library

// Function to initialize Winsock
void initialize_winsock();

// Function to clean up Winsock
void cleanup_winsock();

// Function to handle client connections
void client_handler(SOCKET client_socket);

// Function to start the server
void start_server();

#endif // SERVER_HPP