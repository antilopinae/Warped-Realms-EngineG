#include "PhysicsMoveComponent.hpp"

#include "Actor.hpp"

namespace EngineG {

PhysicsMoveComponent::PhysicsMoveComponent(class Actor* owner, int updateOrder)
    : Component(owner, updateOrder), mSumOfForces(Vector2::Zero), mVelocity(Vector2::Zero), mass(1.0f), mAngularSpeed(0.0f) {
}

void PhysicsMoveComponent::Update(float deltaTime) {
    if (!Math::NearZero(mAngularSpeed)) {
        float rot = mOwner->GetRotation();
        rot += mAngularSpeed * deltaTime;
        mOwner->SetRotation(rot);
    }

    if (!Math::NearZero(mSumOfForces.Length()) || !Math::NearZero(mVelocity.Length())) {
        mVelocity += mSumOfForces * (1 / mass) * deltaTime;
        mSumOfForces = Vector2::Zero;

        Vector2 position = mOwner->GetPosition();
        position += mVelocity * deltaTime;

        // (Screen wrapping code only for asteroids)
        if (position.x < 0.0f) {
            position.x = 1022.0f;
        } else if (position.x > 1024.0f) {
            position.x = 2.0f;
        }

        if (position.y < 0.0f) {
            position.y = 766.0f;
        } else if (position.y > 768.0f) {
            position.y = 2.0f;
        }

        mOwner->SetPosition(position);
    }
}

}  // namespace EngineG
