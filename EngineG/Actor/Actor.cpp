#include "Actor.hpp"

#include "../Component/Component.hpp"
#include "../Component/ScreenComponent.hpp"
#include "../Game.hpp"
#include <algorithm>

namespace EngineG {

Actor::Actor(Game* game) : mState(EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f), mGame(game) {
    mGame->AddActor(this);
}

Actor::~Actor() {
    mGame->RemoveActor(this);
    // Need to delete components
    // Because ~Component calls RemoveComponent, need a different style loop
    while (!mComponents.empty()) {
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime) {
    if (mState == EActive) {
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

void Actor::UpdateComponents(float deltaTime) {
    for (auto comp : mComponents) {
        comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime) {
}

void Actor::ProcessInput(const uint8_t* keyState) {
    if (mState == EActive) {
        // First process input for components
        for (auto comp : mComponents) {
            comp->ProcessInput(keyState);
        }

        ActorInput(keyState);
    }
}

void Actor::ActorInput(const uint8_t* keyState) {
}

void Actor::AddComponent(Component* component) {
    // Find the insertion point in the sorted vector
    // (The first element with a order higher than me)
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); ++iter) {
        if (myOrder < (*iter)->GetUpdateOrder()) {
            break;
        }
    }

    // Inserts element before position of iterator
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end()) {
        mComponents.erase(iter);
    }
}

std::string Actor::GetUniqueActorIdAsString() const {
    // ЗАГЛУШКА: Замените на реальную систему ID
    std::ostringstream oss;
    oss << "ActorP_" << static_cast<const void*>(this);
    return oss.str();
}

Component* Actor::GetComponentToHostWasmScript(const std::string& scriptSlotName) {
    Wasm::LogMessage("EngineG::Actor (" + GetUniqueActorIdAsString() + "): GetComponentToHostWasmScript for slot '" +
                     scriptSlotName + "'.");
    if (scriptSlotName == "SpriteController") {
        for (Component* comp : mComponents) {
            if (dynamic_cast<SpriteComponent*>(comp)) {
                return comp;
            }
        }
    } else if (scriptSlotName == "MovementLogic") {
        for (Component* comp : mComponents) {
            // if (dynamic_cast<YourMoveLogicComponent*>(comp)) return comp;
        }
    }
    Wasm::LogMessage("EngineG::Actor (" + GetUniqueActorIdAsString() + "): No suitable component found for WASM slot '" +
                     scriptSlotName + "'.");
    return nullptr;
}

}  // namespace EngineG
