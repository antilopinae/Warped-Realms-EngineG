#pragma once

#include "Actor.hpp"

namespace EngineG {

class Enemy : public Actor {
public:
    Enemy(class Game* game);
    ~Enemy();
    void UpdateActor(float deltaTime) override;
    class CircleComponent* GetCircle() { return mCircle; }

private:
    class CircleComponent* mCircle;
};

}  // namespace EngineG
