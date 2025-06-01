#include "../engine/GameClient.hpp"
#include "TicTacNet.hpp"

int main() {
    GameClient client;
    client.setGame(std::make_unique<TicTacNet>());
    client.run();
    return 0;
}
