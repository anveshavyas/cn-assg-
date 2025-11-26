#include <string>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 9090

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET clientSock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    char buffer[1024];
    sockaddr_in from{};
    int fromLen = sizeof(from);

    std::cout << "UDP Chat Client started.\n";

    while (true) {
        // Send message
        std::cout << "You: ";
        std::string msg;
        std::getline(std::cin, msg);

        sendto(clientSock, msg.c_str(), msg.size(), 0,
               (sockaddr*)&server, sizeof(server));

        if (msg == "exit") break;

        // Receive reply
        int bytes = recvfrom(clientSock, buffer, sizeof(buffer), 0,
                             (sockaddr*)&from, &fromLen);
        buffer[bytes] = '\0';

        std::cout << "Server: " << buffer << "\n";

        if (strcmp(buffer, "exit") == 0) break;
    }

    closesocket(clientSock);
    WSACleanup();
    return 0;
}
// Compile server:
// g++ udp_chat_server.cpp -o udp_chat_server -lws2_32

// Compile client:
// g++ udp_chat_client.cpp -o udp_chat_client -lws2_32

// Start server:
// .\udp_chat_server

// Start client in another window:
// .\udp_chat_client