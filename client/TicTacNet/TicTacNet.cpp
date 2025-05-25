#include "TicTacNet.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

TicTacNet::TicTacNet() {
    board.fill(" ");
}

void TicTacNet::drawBoard() {
    std::cout << "\n";
    for (int i = 0; i < 9; i++) {
        std::cout << " " << board[i] << " ";
        if (i % 3 != 2) std::cout << "|";
        if (i % 3 == 2 && i != 8) std::cout << "\n-----------\n";
    }
    std::cout << "\n\n";
}

void TicTacNet::makeMove(SOCKET socket) {
    int pos;
    std::cout << "[?] Entrez votre coup (0-8) : ";
    std::cin >> pos;

    if (pos < 0 || pos > 8 || board[pos] != " ") {
        std::cout << "[!] Coup invalide, réessayez.\n";
        return;
    }

    // Place le coup localement
    board[pos] = std::string(1, mySymbol);
    drawBoard();

    // Envoie le coup au serveur
    std::ostringstream oss;
    oss << "MOVE " << pos << "\n";
    std::string moveStr = oss.str();
    send(socket, moveStr.c_str(), moveStr.size(), 0);

    myTurn = false;
}

void TicTacNet::handleServerMessage(const std::string& msg) {
    if (msg.rfind("YOUR_TURN", 0) == 0) {
        myTurn = true;
    }
    else if (msg.rfind("OPPONENT_MOVE", 0) == 0) {
        int pos = std::stoi(msg.substr(14));
        board[pos] = std::string(1, opponentSymbol);
        std::cout << "[~] L’adversaire a joué en " << pos << "\n";
        drawBoard();
    }
    else if (msg.rfind("GAME_OVER", 0) == 0) {
        std::cout << "[✓] Fin de partie : " << msg.substr(10) << "\n";
    }
    else {
        std::cout << "[Serveur] " << msg;
    }
}

void TicTacNet::runLoop(SOCKET socket) {
    char buffer[1024];

    while (true) {
        if (myTurn) {
            makeMove(socket);
        }

        int received = recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) {
            std::cout << "[x] Déconnecté du serveur.\n";
            break;
        }

        buffer[received] = '\0';
        handleServerMessage(buffer);
    }
}
