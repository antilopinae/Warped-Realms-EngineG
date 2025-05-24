#pragma once

#include "Component.hpp"
#include "Math.hpp"

namespace EngineG {

class PhysicsMoveComponent : public Component {
public:
    // Lower update order to update first
    PhysicsMoveComponent(class Actor* owner, int updateOrder = 10);

    void Update(float deltaTime) override;
    void AddForce(Vector2 force) { mSumOfForces += force; }

    void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetVelocity(Vector2 velocity) { mVelocity = velocity; }

    float GetAngularSpeed() const { return mAngularSpeed; }
    Vector2 GetForce() const { return mSumOfForces; }
    Vector2 GetVelocity() const { return mVelocity; }

private:
    Vector2 mSumOfForces;
    Vector2 mVelocity;
    float mass;
    float mAngularSpeed;
};

}  // namespace EngineG
