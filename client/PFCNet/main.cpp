#include "../engine/GameClient.hpp"
#include "PFCGame.hpp"

int main() {
    GameClient client;
    client.setGame(std::make_unique<PFCGame>());
    client.run();
    return 0;
}
