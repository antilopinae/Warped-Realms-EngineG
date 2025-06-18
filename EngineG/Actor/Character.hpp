#pragma once

#include "Actor.hpp"
#include <cstdint>

namespace EngineG {

class Character : public Actor {
public:
    Character(class Game* game);
    void UpdateActor(float deltaTime) override;
    void ProcessKeyboard(const uint8_t* state);
    float GetSpeed() const { return speed; }

private:
    float speed;
    class AnimSpriteComponent* animSprite;
};

}  // namespace EngineG
