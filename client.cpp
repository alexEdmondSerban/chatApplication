#include "client.hpp"
#include <iostream> //For input and output
#include <thread> //For multithreading
#include <cstring> //For memset
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib") // Link with Winsock library
//#include <sys/socket.h> //For socket functions
//#include <arpa/inet.h> //For inet_pton
//#include <unistd.h> //For close function

// Function to initialize Winsock
void initialize_winsock() {
	WSADATA wsa_data; // This structure contains details about the Windows Sockets implementation
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data); // Initialize Winsock
	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << std::endl;
		exit(1); // Exit the program if initialization fails
	}
}

// Function to clean up Winsock
void cleanup_winsock() {
	WSACleanup();
}

//Function to handle receiving messages from the server
void receive_messages(int client_socket) {
	char buffer[1024]; //Buffer to store incoming messages
	while (true) { // Continuous loop to keep receiving messages
		memset(buffer, 0, sizeof(buffer)); // Clear the buffer
		int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);// Receive message

		if (bytes_received <= 0) { // If no message or error, disconnect
			std::cout << "Disconnected from server." << std::endl;
			closesocket(client_socket); // Close the socket
			exit(0); // Exit the program
		}

		buffer[bytes_received] = '\0'; // Null-terminate the received string
		std::cout << "Server: " << buffer << std::endl; // Print the message from the server

	}
}

//Function to handle sending messages to the server
void send_messages(int client_socket) {
	std::string message; // String to store user input 
	while (true) { // Continuous loop to keep sending messages
		std::getline(std::cin, message); // Get user input
		send(client_socket, message.c_str(), message.size(), 0); // Send message to server

	}
}