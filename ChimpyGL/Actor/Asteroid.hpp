#pragma once

#include "Actor.hpp"

namespace EngineG
{

class Asteroid : public Actor
{
  public:
  Asteroid(class Game* game);
  ~Asteroid();

  class CircleComponent* GetCircle()
  {
    return mCircle;
  }

  private:
  class CircleComponent* mCircle;
};


}    // namespace EngineG
