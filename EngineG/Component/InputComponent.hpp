#pragma once

#include "MoveComponent.hpp"
#include <cstdint>

namespace EngineG {

class InputComponent : public MoveComponent {
public:
    // Lower update order to update first
    InputComponent(class Actor* owner);

    void ProcessInput(const uint8_t* keyState) override;

    // Getters/setters for private variables
    float GetMaxForward() const { return mMaxForwardSpeed; }
    float GetMaxAngular() const { return mMaxAngularSpeed; }
    int GetForwardKey() const { return mForwardKey; }
    int GetBackKey() const { return mBackKey; }
    int GetClockwiseKey() const { return mClockwiseKey; }
    int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

    void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
    void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
    void SetForwardKey(int key) { mForwardKey = key; }
    void SetBackKey(int key) { mBackKey = key; }
    void SetClockwiseKey(int key) { mClockwiseKey = key; }
    void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }

    std::vector<Wasm::HostFunctionBinding> GetHostBindingsForWasmSlot(const std::string& slotName) override;

private:
    // The maximum forward/angular speeds
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;
    // Keys for forward/back movement
    int mForwardKey;
    int mBackKey;
    // Keys for angular movement
    int mClockwiseKey;
    int mCounterClockwiseKey;
};

}  // namespace EngineG
