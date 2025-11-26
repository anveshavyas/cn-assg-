#include <iostream>
#include <fstream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9090

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Waiting for file request on port " << PORT << "..." << std::endl;

    int addrlen = sizeof(address);
    SOCKET new_socket = accept(server_fd, (sockaddr*)&address, &addrlen);

    char buffer[1024] = {0};
    int r = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
    buffer[r] = '\0';
    std::string filename = buffer;

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        const char* msg = "ERROR: file not found";
        send(new_socket, msg, strlen(msg), 0);
        closesocket(new_socket);
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize got = file.gcount();
        if (got > 0) send(new_socket, buffer, (int)got, 0);
    }

    std::cout << "File sent successfully." << std::endl;
    file.close();
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
//  Compile File Server:
// g++ file_server.cpp -o file_server -lws2_32

//  Compile File Client:
// g++ file_client.cpp -o file_client -lws2_32

//  Run Server:
// .\file_server.exe

//  Run Client:
// .\file_client.exe
