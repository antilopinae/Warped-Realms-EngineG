#include "Laser.hpp"

#include "Asteroid.hpp"
#include "../Component/CircleComponent.hpp"
#include "../Game.hpp"
#include "../Component/MoveComponent.hpp"
#include "../Component/SpriteComponent.hpp"

namespace EngineG {

Laser::Laser(Game* game) : Actor(game), mDeathTimer(1.0f) {
    // Create a sprite component
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Laser.png"));

    // Create a move component, and set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->SetForwardSpeed(800.0f);

    // Create a circle component (for collision)
    mCircle = new CircleComponent(this);
    mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime) {
    // If we run out of time, laser is dead
    mDeathTimer -= deltaTime;
    if (mDeathTimer <= 0.0f) {
        SetState(EDead);
    } else {
        // Do we intersect with an asteroid?
        for (auto ast : GetGame()->GetAsteroids()) {
            if (Intersect(*mCircle, *(ast->GetCircle()))) {
                // The first asteroid we intersect with,
                // set ourselves and the asteroid to dead
                SetState(EDead);
                ast->SetState(EDead);
                break;
            }
        }
    }
}

}  // namespace EngineG
