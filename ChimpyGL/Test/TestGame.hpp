#pragma once

#include <stdio.h>

#include <cassert>

#include "../Game.hpp"

namespace EngineG::Test
{
class TestGame
{
  public:
  TestGame()
  {
    try
    {
      StartGame();
    }
    catch (...)
    {
      assert(false && "TestGame exception thrown");
    }
    printf("Game passed!\n");
  }

  private:
  void StartGame()
  {
    Game game;
    bool success = game.Initialize();
    if (success)
    {
      game.RunLoop();
    }
    game.Shutdown();
  }
};

}    // namespace EngineG::Test
