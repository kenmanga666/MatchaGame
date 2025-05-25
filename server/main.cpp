#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>

#pragma comment(lib, "ws2_32.lib") // pour linker ws2_32

std::vector<SOCKET> queue;
std::mutex queue_mutex;

void handleMatch(SOCKET player1, SOCKET player2) {
    std::cout << "[*] Match lancé entre deux joueurs." << std::endl;

    const char* start_msg = "MATCH_START\n";
    send(player1, start_msg, strlen(start_msg), 0);
    send(player2, start_msg, strlen(start_msg), 0);

    auto forwardMessages = [](SOCKET sender, SOCKET receiver) {
        char buffer[1024];
        int bytesReceived;

        while ((bytesReceived = recv(sender, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytesReceived] = '\0';
            send(receiver, buffer, bytesReceived, 0);
        }

        // Si on arrive ici, le client s’est déconnecté
        const char* dc_msg = "DISCONNECT\n";
        send(receiver, dc_msg, strlen(dc_msg), 0);
        closesocket(sender);
        closesocket(receiver);
    };

    std::thread(forwardMessages, player1, player2).detach();
    std::thread(forwardMessages, player2, player1).detach();
}


void handleClient(SOCKET clientSocket) {
    std::cout << "[+] Nouveau client connecté." << std::endl;

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        queue.push_back(clientSocket);

        // Si deux clients sont en attente, lancer une partie
        if (queue.size() >= 2) {
            SOCKET p1 = queue[0];
            SOCKET p2 = queue[1];
            queue.erase(queue.begin(), queue.begin() + 2);
            std::thread(handleMatch, p1, p2).detach();
        }
        else {
            const char* wait_msg = "En attente d’un autre joueur...\n";
            send(clientSocket, wait_msg, strlen(wait_msg), 0);
        }
    }
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur socket()\n";
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "[*] Serveur MatchaGame lancé sur le port 5000...\n";

    while (true) {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket != INVALID_SOCKET) {
            std::thread(handleClient, clientSocket).detach();
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
