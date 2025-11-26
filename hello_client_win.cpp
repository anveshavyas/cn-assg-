#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[1024] = {0};
    const char* hello = "Hello from client";

    WSAStartup(MAKEWORD(2,2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connect failed\n";
        return 1;
    }

    send(sock, hello, strlen(hello), 0);
    std::cout << "Hello message sent\n";

    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "Server: " << buffer << "\n";

    closesocket(sock);
    WSACleanup();
    return 0;
}
//  1. Compile the Windows Server Program
// g++ hello_server_win.cpp -o hello_server_win -lws2_32

// Run the server:
// .\hello_server_win.exe

//  2. Compile the Windows Client Program
// g++ hello_client_win.cpp -o hello_client_win -lws2_32

// Run the client:
// .\hello_client_win.exe