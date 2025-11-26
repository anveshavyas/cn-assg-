#include <winsock2.h>
#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[1024];

    // Initialize WinSock
    WSAStartup(MAKEWORD(2,2), &wsa);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Server settings
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server!\n\n";

    std::string msg;

    // Chat loop
    while (true) {
        std::cout << "You: ";
        std::getline(std::cin, msg);

        send(sock, msg.c_str(), msg.length(), 0);

        if (msg == "exit") {
            std::cout << "Leaving chat...\n";
            break;
        }

        // Receive reply
        int val = recv(sock, buffer, sizeof(buffer), 0);
        if (val <= 0) break;
        buffer[val] = '\0';

        std::cout << "Server: " << buffer << "\n";
    }

    closesocket(sock);
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