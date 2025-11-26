#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1024] = {0};
    const char* hello = "Hello from server";

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    listen(server_fd, 3);
    std::cout << "Server listening on port " << PORT << "...\n";

    c = sizeof(struct sockaddr_in);
    new_socket = accept(server_fd, (struct sockaddr*)&client, &c);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        return 1;
    }

    recv(new_socket, buffer, sizeof(buffer), 0);
    std::cout << "Client: " << buffer << "\n";

    send(new_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent\n";

    closesocket(new_socket);
    closesocket(server_fd);
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