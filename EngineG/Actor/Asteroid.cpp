#include "Asteroid.hpp"

#include "../Component/CircleComponent.hpp"
#include "../Game.hpp"
#include "../Math/Math.hpp"
#include "../Component/MoveComponent.hpp"
#include "../Component/SpriteComponent.hpp"

namespace EngineG {

Asteroid::Asteroid(Game* game) : Actor(game), mCircle(nullptr) {
    // Initialize to random position/orientation
    Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
    SetPosition(randPos);

    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    // Create a sprite component
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Asteroid.png"));

    // Create a move component, and set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->SetForwardSpeed(150.0f);

    // Create a circle component (for collision)
    mCircle = new CircleComponent(this);
    mCircle->SetRadius(40.0f);

    // Add to mAsteroids in game
    game->AddAsteroid(this);
}

Asteroid::~Asteroid() {
    GetGame()->RemoveAsteroid(this);
}

}  // namespace EngineG
