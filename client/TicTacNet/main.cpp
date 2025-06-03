#include "../engine/GameClient.hpp"
#include "TicTacNet.hpp"
#include <windows.h>

class TicTacNetWrapper : public Game {
public:
    void runLoop(SOCKET socket) override {
        // 1. Envoie le nom du jeu
        send(socket, "GAME TicTacNet_CLI\n", strlen("GAME TicTacNet_CLI\n"), 0);

        // 2. Attend MATCH_START
        char buffer[1024];
        std::string partial;
        while (true) {
            int received = recv(socket, buffer, sizeof(buffer) - 1, 0);
            if (received <= 0) {
                std::cerr << "[x] Déconnecté.\n";
                return;
            }

            buffer[received] = '\0';
            partial += buffer;

            size_t pos;
            while ((pos = partial.find('\n')) != std::string::npos) {
                std::string msg = partial.substr(0, pos);
                partial.erase(0, pos + 1);

                if (msg == "MATCH_START") {
                    std::cout << "[✓] Match trouvé !\n";
                    TicTacNet game;
                    game.runLoop(socket);
                    return;
                }
                else {
                    std::cout << "[Serveur] " << msg << "\n";
                }
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    GameClient client;
    client.setGame(std::make_unique<TicTacNetWrapper>());
    client.run("127.0.0.1", 5000);
    return 0;
}
