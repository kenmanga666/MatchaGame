#pragma once

#include <winsock2.h>
#include <thread>
#include <string>
#include <iostream>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

class Game {
public:
    virtual ~Game() = default;
    virtual void runLoop(SOCKET socket) = 0;
};

class GameClient {
private:
    std::unique_ptr<Game> game;

public:
    void setGame(std::unique_ptr<Game> g) {
        game = std::move(g);
    }

    void run(const std::string& server_ip = "127.0.0.1", int port = 5000) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

        if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Erreur de connexion au serveur.\n";
            return;
        }

        std::cout << "[✓] Connecté au serveur.\n";

        if (game) {
            game->runLoop(sock);
        }
        else {
            std::cerr << "Aucun jeu défini !\n";
        }

        closesocket(sock);
        WSACleanup();
    }
};
