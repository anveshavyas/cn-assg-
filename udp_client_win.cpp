#include <iostream>
#include <winsock2.h>
using namespace std;

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    sockaddr_in servaddr{};
    int len = sizeof(servaddr);
    char buffer[BUF_SIZE];

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cout << "WSAStartup failed\n";
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        cout << "Socket creation failed\n";
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    const char *msg = "Hello from Client!";
    sendto(sockfd, msg, strlen(msg), 0, (sockaddr*)&servaddr, len);

    recvfrom(sockfd, buffer, BUF_SIZE, 0, (sockaddr*)&servaddr, &len);
    cout << "Server: " << buffer << endl;

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
// g++ udp_server_win.cpp -o server -lws2_32
// g++ udp_client_win.cpp -o client -lws2_32

// .\server
// .\client