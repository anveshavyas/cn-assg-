#include <iostream>
#include <winsock2.h>
using namespace std;

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    sockaddr_in servaddr{}, cliaddr{};
    int len = sizeof(cliaddr);
    char buffer[BUF_SIZE];

    // Initialize Winsock
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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        cout << "Bind failed\n";
        return 1;
    }

    cout << "UDP Server running... Waiting for messages.\n";
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (sockaddr*)&cliaddr, &len);
    cout << "Client: " << buffer << endl;

    const char *msg = "Hello from Server!";
    sendto(sockfd, msg, strlen(msg), 0, (sockaddr*)&cliaddr, len);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
// g++ udp_server_win.cpp -o server -lws2_32
// g++ udp_client_win.cpp -o client -lws2_32

// .\server
// .\client