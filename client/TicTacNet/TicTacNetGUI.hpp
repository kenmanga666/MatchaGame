#pragma once
#include "../engine/GameClient.hpp"
#include <SFML/Graphics.hpp>
#include <array>

class TicTacNetGUI : public Game {
public:
    TicTacNetGUI() = default;
    TicTacNetGUI(const TicTacNetGUI&) = delete;
    TicTacNetGUI& operator=(const TicTacNetGUI&) = delete;
    TicTacNetGUI(TicTacNetGUI&&) = default;
    TicTacNetGUI& operator=(TicTacNetGUI&&) = default;
    void runLoop(SOCKET socket) override;

private:
	bool waitingForPlayer = true;
    void drawBoard();
    void drawSymbols();
    void processClick(int x, int y);
    void handleServerMessage(const std::string& msg);
    std::string checkGameStatus();

    sf::RenderWindow window;
    std::array<std::string, 9> board;
    char mySymbol = 'X';
    char opponentSymbol = 'O';
    bool myTurn = false;
    bool gameOver = false;
    SOCKET sock;
    sf::Font font;
    std::unique_ptr<sf::Text> statusText;
    std::unique_ptr<sf::Text> endText;
    std::unique_ptr<sf::Text> replayLabel;
    std::unique_ptr<sf::Text> quitLabel;
    sf::RectangleShape replayButton;
    sf::RectangleShape quitButton;
};
