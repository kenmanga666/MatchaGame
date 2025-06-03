#include "TicTacNetGUI.hpp"
#include <iostream>Add commentMore actions
#include <sstream>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

void TicTacNetGUI::runLoop(SOCKET socket) {
    this->sock = socket;
    u_long mode = 1;
    ioctlsocket(socket, FIONBIO, &mode); // rend la socket non bloquante (une seule fois)
    board.fill(" ");
    window.create(sf::VideoMode({ 600, 640 }), "TicTacNet");

    // Chargement police
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "[!] Erreur chargement police\n";
        return;
    }

    // Texte de statut -> À toi de jouer / Tour adverse
    statusText = std::make_unique<sf::Text>(font, "", 24);
    statusText->setFillColor(sf::Color::Black);
    statusText->setPosition(sf::Vector2f(10.f, 10.f));

    // Texte de fin
    endText = std::make_unique<sf::Text>(font, "", 48);
    endText->setFillColor(sf::Color::Red);
    endText->setPosition(sf::Vector2f(100.f, 250.f));

    // Bouton rejouer
    replayButton.setSize(sf::Vector2f(200.f, 50.f));
    replayButton.setPosition(sf::Vector2f(100.f, 350.f));
    replayButton.setFillColor(sf::Color(200, 200, 200));
    replayLabel = std::make_unique<sf::Text>(font, "Rejouer", 20);
    replayLabel->setFillColor(sf::Color::Black);
    replayLabel->setPosition(sf::Vector2f(130.f, 360.f));

    // Bouton quitter
    quitButton.setSize(sf::Vector2f(200.f, 50.f));
    quitButton.setPosition(sf::Vector2f(320.f, 350.f));
    quitButton.setFillColor(sf::Color(200, 200, 200));
    quitLabel = std::make_unique<sf::Text>(font, "Quitter", 20);
    quitLabel->setFillColor(sf::Color::Black);
    quitLabel->setPosition(sf::Vector2f(370.f, 360.f));

    // Envoie le nom du jeu
    send(socket, "GAME TicTacNet_GUI\n", strlen("GAME TicTacNet_GUI\n"), 0);

    // Attente du MATCH_START
    char buffer[1024];
    std::string partial;
    while (window.isOpen()) {
        // Réception messages serveur
        int received = recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0';
            partial += buffer;

            size_t pos;
            while ((pos = partial.find('\n')) != std::string::npos) {
                std::string msg = partial.substr(0, pos);
                partial.erase(0, pos + 1);
                handleServerMessage(msg);
            }
        }

        // Gère les events SFML
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::MouseButtonPressed>() && myTurn && !gameOver) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                int x = static_cast<int>(worldPos.x);
                int y = static_cast<int>(worldPos.y);
                processClick(x, y);
            }
            else if (event->is<sf::Event::MouseButtonPressed>() && gameOver) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                int x = static_cast<int>(worldPos.x);
                int y = static_cast<int>(worldPos.y);

                if (replayButton.getGlobalBounds().contains(sf::Vector2f(x, y))) {
                    std::cout << "[~] Rejouer demandé.\n";
                    window.close(); // ferme cette fenêtre
                    system("start TicTacNet_GUI.exe"); // relance (fonctionne si dans path)
                }

                if (quitButton.getGlobalBounds().contains(sf::Vector2f(x, y))) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::White);
        drawBoard();
        drawSymbols();

        // Texte de statut
        if (!gameOver) {
            if (waitingForPlayer)
                statusText->setString("En attente d un autre joueur...");
            else
                statusText->setString(myTurn ? "À toi de jouer" : "Tour adverse");
            window.draw(*statusText);
        }
        else {
            window.draw(*endText);
            window.draw(replayButton);
            window.draw(quitButton);
            window.draw(*replayLabel);
            window.draw(*quitLabel);
        }
        window.display();
    }
}

void TicTacNetGUI::drawBoard() {
    sf::RectangleShape line(sf::Vector2f(600, 5));
    line.setFillColor(sf::Color::Black);

    // lignes horizontales
    for (int i = 1; i <= 2; ++i) {
        line.setPosition(sf::Vector2f(static_cast<float>(0), static_cast<float>(i * 200)));
        window.draw(line);
    }

    // lignes verticales
    line.setSize(sf::Vector2f(5, 600));
    for (int i = 1; i <= 2; ++i) {
        line.setPosition(sf::Vector2f(static_cast<float>(i * 200), static_cast<float>(40)));
        window.draw(line);
    }
}

void TicTacNetGUI::drawSymbols() {
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "[!] Police non trouvée\n";
            return;
        }
        loaded = true;
    }
    sf::Text text(font, "", 150);
    text.setCharacterSize(150);
    text.setFillColor(sf::Color::Black);

    for (int i = 0; i < 9; ++i) {
        if (board[i] == " ") continue;
        text.setString(board[i]);
        int col = i % 3;
        int row = i / 3;
        text.setPosition(sf::Vector2f(static_cast<float>(col * 200 + 50), static_cast<float>(row * 200 + 30)));
        window.draw(text);
    }
}

void TicTacNetGUI::processClick(int x, int y) {
    int col = x / 200;
    int row = y / 200;
    int pos = row * 3 + col;

    if (pos < 0 || pos > 8 || board[pos] != " ") return;

    board[pos] = std::string(1, mySymbol);
    myTurn = false;

    std::ostringstream oss;
    oss << "MOVE " << pos << "\n";
    send(sock, oss.str().c_str(), oss.str().size(), 0);
    send(sock, "YOUR_TURN\n", strlen("YOUR_TURN\n"), 0);

    std::string status = checkGameStatus();
    if (!status.empty()) {
        std::string msg = "GAME_OVER " + status + "\n";
        send(sock, msg.c_str(), msg.size(), 0);
        gameOver = true;
    }
}

void TicTacNetGUI::handleServerMessage(const std::string& msg) {
    if (msg == "MATCH_START") {
        std::cout << "[✓] Match trouvé.\n";
        waitingForPlayer = false;
    }
    else if (msg == "YOUR_TURN") {
        myTurn = true;
    }
    else if (msg.rfind("MOVE", 0) == 0) {
        int pos = std::stoi(msg.substr(5));
        board[pos] = std::string(1, opponentSymbol);
    }
    else if (msg.rfind("GAME_OVER", 0) == 0) {
        gameOver = true;
        std::string result = msg.substr(10);
        if (result == "WIN") endText->setString("Défaite..."); // -> l'adversaire renvoie "WIN" si il a gagné donc tu as perdu
        else endText->setString("Égalité");
    }
    else {
        std::cout << "[Serveur] " << msg << "\n";
    }
}

std::string TicTacNetGUI::checkGameStatus() {
    int win[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for (auto& c : win) {
        if (board[c[0]] != " " && board[c[0]] == board[c[1]] && board[c[1]] == board[c[2]]) {
            // on set le texte de fin pour afficher notre victoire
            endText->setString(board[c[0]] == std::string(1, mySymbol) ? "Victoire !" : "Défaite...");

            // on a gagné donc on retourne l'information à l'adversaire
            return "WIN";
        }
    }

    bool full = true;
    for (auto& cell : board)
        if (cell == " ") full = false;

    if (full) {
        endText->setString("Égalité");
        return "DRAW";
    }

	return ""; // pas de fin de jeu
}