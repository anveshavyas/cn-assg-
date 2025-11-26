#include <iostream>
#include <fstream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9090

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    std::string filename = "sample.txt"; // file on server
    send(sock, filename.c_str(), (int)filename.size(), 0);

    std::ofstream outfile("received_" + filename, std::ios::binary);
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        outfile.write(buffer, bytesRead);
    }

    outfile.close();
    closesocket(sock);
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
