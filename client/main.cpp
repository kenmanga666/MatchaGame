#include <iostream>
#include <winsock2.h>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

void receiveLoop(SOCKET socket) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cout << "[x] Déconnexion du serveur.\n";
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << "[Serveur] " << buffer;
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur de connexion au serveur.\n";
        return 1;
    }

    std::cout << "[✓] Connecté au serveur.\n";

    std::thread receiver(receiveLoop, sock);
    receiver.detach();

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "/quit") break;
        send(sock, input.c_str(), input.size(), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
