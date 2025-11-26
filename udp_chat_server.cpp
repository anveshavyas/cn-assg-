#include <string>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9090

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server{}, client{};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(serverSock, (sockaddr*)&server, sizeof(server));

    std::cout << "UDP Chat Server running on port " << PORT << "...\n";

    char buffer[1024];
    int clientLen = sizeof(client);

    while (true) {
        // Receive message
        int bytes = recvfrom(serverSock, buffer, sizeof(buffer), 0,
                             (sockaddr*)&client, &clientLen);
        buffer[bytes] = '\0';

        std::cout << "\nClient: " << buffer << "\n";

        if (strcmp(buffer, "exit") == 0) break;

        // Send reply
        std::cout << "You: ";
        std::string msg;
        std::getline(std::cin, msg);

        sendto(serverSock, msg.c_str(), msg.size(), 0,
               (sockaddr*)&client, clientLen);

        if (msg == "exit") break;
    }

    closesocket(serverSock);
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