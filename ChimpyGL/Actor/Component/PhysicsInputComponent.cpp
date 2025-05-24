#include "PhysicsInputComponent.hpp"

#include "Actor.hpp"

namespace EngineG {

PhysicsInputComponent::PhysicsInputComponent(class Actor* owner)
    : PhysicsMoveComponent(owner), mForwardKey(0), mBackKey(0), mClockwiseKey(0), mCounterClockwiseKey(0) {
}

void PhysicsInputComponent::ProcessInput(const uint8_t* keyState) {
    // Calculate forward speed for MoveComponent
    float forwardSpeed = 0.0f;
    if (keyState[mForwardKey]) {
        forwardSpeed += mMaxForwardSpeed;
    }
    if (keyState[mBackKey]) {
        forwardSpeed -= mMaxForwardSpeed;
    }
    AddForce(mOwner->GetForward() * forwardSpeed);

    // Calculate angular speed for MoveComponent
    float angularSpeed = 0.0f;
    if (keyState[mClockwiseKey]) {
        angularSpeed += mMaxAngularSpeed;
    }
    if (keyState[mCounterClockwiseKey]) {
        angularSpeed -= mMaxAngularSpeed;
    }
    SetAngularSpeed(angularSpeed);
}

}  // namespace EngineG
