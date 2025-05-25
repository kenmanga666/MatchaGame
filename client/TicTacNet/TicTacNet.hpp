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

public:
    TicTacNet();
    void runLoop(SOCKET socket) override;
    void drawBoard();
    void makeMove(SOCKET socket);
    void handleServerMessage(const std::string& msg);
};
