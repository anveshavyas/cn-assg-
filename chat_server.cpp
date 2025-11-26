#include <winsock2.h>
#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1024];

    // Initialize WinSock
    WSAStartup(MAKEWORD(2,2), &wsa);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Server info
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    // Listen
    listen(server_fd, 3);
    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept a connection
    c = sizeof(struct sockaddr_in);
    new_socket = accept(server_fd, (struct sockaddr*)&client, &c);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        return 1;
    }

    std::cout << "Client connected!\n\n";

    std::string msg;

    // Chat loop
    while (true) {
        // Receive message
        int val = recv(new_socket, buffer, sizeof(buffer), 0);
        if (val <= 0) break;
        buffer[val] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Client left the chat.\n";
            break;
        }

        std::cout << "Client: " << buffer << "\n";

        // Read server message from keyboard
        std::cout << "You: ";
        std::getline(std::cin, msg);

        send(new_socket, msg.c_str(), msg.length(), 0);

        if (msg == "exit") {
            std::cout << "Closing chat...\n";
            break;
        }
    }

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
// Compile server:
// g++ chat_server.cpp -o chat_server -lws2_32

// Compile client:
// g++ chat_client.cpp -o chat_client -lws2_32

// Run Server FIRST:
// .\chat_server.exe

// Run Client in another terminal:
// .\chat_client.exe