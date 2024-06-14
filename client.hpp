#ifndef CLIENT_HPP
#define CLIENT_HPP

//Function to handle receiving messages from the server
void receive_messages(int client_socket);

//Function to handle sending messages to the server
void send_messages(int client_socket);

void initialize_winsock();

void cleanup_winsock();

#endif // CLIENT_HPP