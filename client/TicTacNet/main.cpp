#include "../engine/GameClient.hpp"
#include "MorpionGame.hpp"

int main() {
    GameClient client;
    client.setGame(std::make_unique<MorpionGame>());
    client.run();
    return 0;
}
