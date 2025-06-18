#include "InputComponent.hpp"

#include "../Actor/Actor.hpp"
#include "../Game.hpp"

namespace EngineG {

InputComponent::InputComponent(class Actor* owner)
    : MoveComponent(owner), mForwardKey(0), mBackKey(0), mClockwiseKey(0), mCounterClockwiseKey(0) {
    AttachWasmScriptToSelf("Input.ModuleA", ASSETS_DIR "moduleA_c.wasm");
}

std::vector<Wasm::HostFunctionBinding> InputComponent::GetHostBindingsForWasmSlot(const std::string& slotName) {
    return {Wasm::HostFunctionBinding(
        "env", "getInput", Wasm::HostFuncVariant([](wasmtime::Caller&) { std::cout << "Hello input!" << std::endl; }))};
}

void InputComponent::ProcessInput(const uint8_t* keyState) {
    // Calculate forward speed for MoveComponent
    float forwardSpeed = 0.0f;
    if (keyState[mForwardKey]) {
        forwardSpeed += mMaxForwardSpeed;
    }
    if (keyState[mBackKey]) {
        forwardSpeed -= mMaxForwardSpeed;
    }
    SetForwardSpeed(forwardSpeed);

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
