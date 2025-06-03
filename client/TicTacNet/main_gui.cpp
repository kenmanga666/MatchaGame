#include "../engine/GameClient.hpp"
#include "TicTacNetGUI.hpp"

int main() {
    GameClient client;
    client.setGame(std::make_unique<TicTacNetGUI>());
    client.run("127.0.0.1", 5000);
    return 0;
}
