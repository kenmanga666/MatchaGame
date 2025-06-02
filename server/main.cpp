#include <winsock2.h>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

std::unordered_map<std::string, std::vector<SOCKET>> waitingPlayers;
std::mutex waitingMutex;

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int len = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (len <= 0) {
        std::cout << "[!] Client déconnecté avant de dire quel jeu il veut.\n";
        closesocket(clientSocket);
        return;
    }

    buffer[len] = '\0';
    std::string message(buffer);
    std::cout << "[->] Message reçu : " << message;

    // Vérifie que le message commence par "GAME "
    if (message.rfind("GAME ", 0) != 0) {
        std::cout << "[!] Mauvais message d’identification. Déconnexion.\n";
        closesocket(clientSocket);
        return;
    }

    // Extrait le nom du jeu
    std::string gameName = message.substr(5);
    if (!gameName.empty() && gameName.back() == '\n')
        gameName.pop_back();

    std::cout << "[*] Client veut jouer à : " << gameName << "\n";

    // File d’attente
    std::lock_guard<std::mutex> lock(waitingMutex);
    auto& queue = waitingPlayers[gameName];
    queue.push_back(clientSocket);

    if (queue.size() >= 2) {
        SOCKET p1 = queue[0];
        SOCKET p2 = queue[1];
        queue.erase(queue.begin(), queue.begin() + 2);

        std::thread([](SOCKET a, SOCKET b) {
            const char* start = "MATCH_START\n";
            send(a, start, strlen(start), 0);
            send(b, start, strlen(start), 0);
            send(a, "YOUR_TURN\n", strlen("YOUR_TURN\n"), 0);

            auto relay = [](SOCKET from, SOCKET to) {
                char buffer[1024];
                int len;
                while ((len = recv(from, buffer, sizeof(buffer) - 1, 0)) > 0) {
                    buffer[len] = '\0';
                    send(to, buffer, len, 0);
                }
                shutdown(from, SD_BOTH);
                shutdown(to, SD_BOTH);
                closesocket(from);
                closesocket(to);
                };

            std::thread(relay, a, b).detach();
            std::thread(relay, b, a).detach();
            }, p1, p2).detach();
    }
    else {
        const char* waitMsg = "WAITING\n";
        send(clientSocket, waitMsg, strlen(waitMsg), 0);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Échec de l'initialisation de Winsock.\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Échec de la création du socket serveur.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5000);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Échec du bind du socket serveur.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Échec de l'écoute sur le socket serveur.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[*] MatchaGame lancé sur le port 5000. En attente de connexions clients...\n";
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Échec de l'acceptation d'une connexion.\n";
            continue;
        }
        std::cout << "[+] Client connecté.\n";
        std::thread(handleClient, clientSocket).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
	return 0;
}

