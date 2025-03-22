#pragma once

#include "Actor.hpp"
#include <cstdint>

namespace EngineG{

class Ship : public Actor
{
public:
    Ship(class Game* game);
    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keyState) override;

    // void ProcessKeyboard(const uint8_t* state);
    float GetRightSpeed() const { return mRightSpeed; }
    float GetDownSpeed() const { return mDownSpeed; }
private:
    bool isVisible;

    float mRightSpeed;
    float mDownSpeed;

    float mLaserCooldown;
    float mDisappearCooldown;

    class CircleComponent* mCircle;
    class InputComponent* mInputComponent;
    // class PhysicsInputComponent* mInputComponent;
    class AnimSpriteComponent* mAnimSprite;
};

}
