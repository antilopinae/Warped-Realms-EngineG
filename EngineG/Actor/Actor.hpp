#pragma once

#include "../Math/Math.hpp"
using namespace EngineG::Math;

#include <cstdint>
#include <vector>

#ifdef DEBUG
#define ASSETS_DIR "../../Assets/"

#else
#define ASSETS_DIR "../Assets/"

#endif

namespace EngineG {

class Actor {
public:
    enum State { EActive, EPaused, EDead };

    Actor(class Game* game);
    virtual ~Actor();

    // Update function called from Game (not overridable)
    void Update(float deltaTime);
    // Updates all the components attached to the actor (not overridable)
    void UpdateComponents(float deltaTime);
    // Any actor-specific update code (overridable)
    virtual void UpdateActor(float deltaTime);

    // ProcessInput function called from Game (not overridable)
    void ProcessInput(const uint8_t* keyState);
    // Any actor-specific input code (overridable)
    virtual void ActorInput(const uint8_t* keyState);

    // Getters/setters
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& pos) { mPosition = pos; }
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; }
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; }

    Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }

    class Game* GetGame() { return mGame; }

    // Add/remove components
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    virtual std::string GetUniqueActorIdAsString() const;
    virtual Component* GetComponentToHostWasmScript(const std::string& scriptSlotName);

    template <typename T>
    T* GetComponentOfType() {
        for (auto c : mComponents) {
            T* t = dynamic_cast<T*>(c);
            if (t != nullptr) {
                return t;
            }
        }
        return nullptr;
    }

private:
    // Actor's state
    State mState;
    // std::string mActorId;

    // Transform
    Vector2 mPosition;
    float mScale;
    float mRotation;

    std::vector<class Component*> mComponents;
    class Game* mGame;
};

}  // namespace EngineG
