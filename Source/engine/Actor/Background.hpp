#pragma once

#include "Actor.hpp"

namespace EngineG{

class Background: public Actor
{
public:
  Background(class Game* game);

private:
  std::vector<class TileMapComponent*> mLayers;
};

}