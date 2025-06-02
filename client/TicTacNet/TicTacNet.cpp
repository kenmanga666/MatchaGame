#include "TicTacNet.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <windows.h>

TicTacNet::TicTacNet() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
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

bool TicTacNet::makeMove(SOCKET socket) {
    int pos = -1;

    // Boucle jusqu'à coup valide
    while (true) {
        std::cout << "[?] Entrez votre coup (0-8) : ";
        std::cout.flush();
        std::cin >> pos;

        if (std::cin.fail()) {
            std::cin.clear(); // nettoie l’état d’erreur
            std::cin.ignore(1000, '\n'); // vide le buffer
            std::cout << "[!] Entrée invalide.\n";
            continue;
        }

        if (pos < 0 || pos > 8 || board[pos] != " ") {
            std::cout << "[!] Coup invalide, réessayez.\n";
            continue;
        }

        break; // coup valide
    }

    board[pos] = std::string(1, mySymbol);
    drawBoard();

    std::string result = checkGameStatus();
    if (!result.empty()) {
        std::string msg = "GAME_OVER " + result + "\n";
        send(socket, msg.c_str(), msg.size(), 0);
        std::cout << "[✓] Partie terminée (" << result << ")\n";
		gameOver = true;
		return true;
    }

    std::ostringstream oss;
    oss << "MOVE " << pos << "\n";
    std::string moveStr = oss.str();
    send(socket, moveStr.c_str(), moveStr.size(), 0);
    send(socket, "YOUR_TURN\n", strlen("YOUR_TURN\n"), 0);

    myTurn = false;
    return false;
}

void TicTacNet::handleServerMessage(const std::string& msg) {
    if (msg.rfind("YOUR_TURN", 0) == 0) {
        myTurn = true;
    }
    else if (msg.rfind("MOVE", 0) == 0) {
        int pos = std::stoi(msg.substr(5));
        board[pos] = std::string(1, opponentSymbol);
        std::cout << "[~] L’adversaire a joué en " << pos << "\n";
        drawBoard();
    }
    else if (msg.rfind("GAME_OVER", 0) == 0) {
        std::string outcome = msg.substr(10);
        if (outcome == "WIN\n") outcome = "LOSE"; // Si on reçoit "WIN" c'est que l'adversaire à gagné -> on a perdu
		else if (outcome == "DRAW\n") outcome = "DRAW"; // Égalité, on enlève le \n pour l'affichage
        std::cout << "[✓] Partie terminée (" << outcome << ")\n";
        gameOver = true;
    }
}

std::string TicTacNet::checkGameStatus() {
    // Combinaisons gagnantes
    int winningCombos[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // lignes
        {0,3,6}, {1,4,7}, {2,5,8}, // colonnes
        {0,4,8}, {2,4,6}           // diagonales
    };

    for (auto& combo : winningCombos) {
        const std::string& a = board[combo[0]];
        const std::string& b = board[combo[1]];
        const std::string& c = board[combo[2]];

        if (a == b && b == c && a != " ") {
            // Quelqu’un a gagné
            return (a[0] == mySymbol) ? "WIN" : "LOSE";
        }
    }

    // Vérifie égalité
    bool full = true;
    for (const auto& cell : board) {
        if (cell == " ") {
            full = false;
            break;
        }
    }

    if (full) return "DRAW";

    return "";
}

void TicTacNet::runLoop(SOCKET socket) {
    while (!gameOver) {
        if (myTurn) {
            if (makeMove(socket)) break; // Partie terminée
        }
        else {
            std::string msg;
            char buffer[256];
            int bytesReceived = recv(socket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0'; // Terminer la chaîne
                msg = std::string(buffer);
                handleServerMessage(msg);
            } else if (bytesReceived == 0) {
                std::cout << "[!] Connexion perdue.\n";
                gameOver = true;
            } else {
                std::cerr << "[!] Erreur de réception : " << WSAGetLastError() << "\n";
            }
        }
    }

    std::cout << "[*] Partie terminée. Appuyez sur Entrée pour quitter.\n";
    std::cin.ignore();
    std::cin.get();
}