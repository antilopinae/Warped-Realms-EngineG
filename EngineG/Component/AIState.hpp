#pragma once

namespace EngineG {

class AIState {
public:
    AIState(class AIComponent* owner) : mOwner(owner) {}
    // State-specific behavior
    virtual void Update(float deltaTime) = 0;
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    // Getter for string name of state
    virtual const char* GetName() const = 0;

protected:
    class AIComponent* mOwner;
};

class AIPatrol : public AIState {
public:
    AIPatrol(class AIComponent* owner) : AIState(owner) {}

    // Override with behaviors for this state
    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;

    const char* GetName() const override { return "Patrol"; }
};

class AIDeath : public AIState {
public:
    AIDeath(class AIComponent* owner) : AIState(owner) {}

    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;

    const char* GetName() const override { return "Death"; }
};

class AIAttack : public AIState {
public:
    AIAttack(class AIComponent* owner) : AIState(owner) {}

    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;

    const char* GetName() const override { return "Attack"; }
};

}  // namespace EngineG
