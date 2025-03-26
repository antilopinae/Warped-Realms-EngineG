#include <Game.hpp>
#include <iostream>

void ExQuickJs();

int main() {
    ExQuickJs();
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
