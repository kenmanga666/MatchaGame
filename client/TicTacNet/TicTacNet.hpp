#pragma once

#include "../engine/GameClient.hpp"
#include <array>
#include <string>

class TicTacNet : public Game {
private:
    std::array<std::string, 9> board;
    bool myTurn = false;
    char mySymbol = 'X';
    char opponentSymbol = 'O';
    bool gameOver = false;

public:
    TicTacNet();
    void runLoop(SOCKET socket) override;
    void drawBoard();
    bool makeMove(SOCKET socket);
    void handleServerMessage(const std::string& msg);
    std::string checkGameStatus();
};
