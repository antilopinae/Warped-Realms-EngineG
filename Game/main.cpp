#include <Game.hpp>
#include <iostream>

void ExWasm();

int main() {
    ExWasm();

    EngineG::Game game;
    if (game.Initialize()) {
        game.RunLoop();
    } else {
       std::cerr << "Game initialization failed!" << std::endl;
       return -1;
    }
   game.Shutdown();
   return 0;
}
