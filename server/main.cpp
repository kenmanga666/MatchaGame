#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib") // pour linker ws2_32

std::vector<SOCKET> queue;
std::mutex queue_mutex;

void handleMatch(SOCKET player1, SOCKET player2) {
    const char* start = "MATCH_START\n";
    send(player1, start, strlen(start), 0);
    send(player2, start, strlen(start), 0);

    send(player1, "YOUR_TURN\n", strlen("YOUR_TURN\n"), 0);

    auto forward = [](SOCKET from, SOCKET to) {
        char buffer[1024];
        int len;
        while ((len = recv(from, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[len] = '\0';

            // Si c’est un coup : MOVE x
            if (strncmp(buffer, "MOVE", 4) == 0) {
                send(to, ("OPPONENT_" + std::string(buffer)).c_str(), len + 9, 0);
                send(to, "YOUR_TURN\n", strlen("YOUR_TURN\n"), 0);
            }

            // Si c’est la fin du jeu
            else if (strncmp(buffer, "GAME_OVER", 9) == 0) {
                std::string msg(buffer, len);
                std::string outcome = msg.substr(10);
				outcome.erase(outcome.find_last_not_of("\n") + 1); // Enlever les espaces à la fin
				// print outcome for debugging
				//std::cout << "outcome : " << outcome << "verif_espace" << std::endl;

                std::string forSender = "GAME_OVER " + outcome;
                std::string forReceiver;

                if (outcome == "WIN") forReceiver = "GAME_OVER LOSE";
                else if (outcome == "LOSE") forReceiver = "GAME_OVER WIN";
                else forReceiver = "GAME_OVER DRAW\n";

                send(from, forSender.c_str(), forSender.size(), 0);
                send(to, forReceiver.c_str(), forReceiver.size(), 0);

                break;
            }
        }

        shutdown(from, SD_BOTH);
        shutdown(to, SD_BOTH);
        closesocket(from);
        closesocket(to);
    };

    std::thread(forward, player1, player2).detach();
    std::thread(forward, player2, player1).detach();
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

		// Afficher la déconnection du client
		std::cout << "[*] Client déconnecté." << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

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
