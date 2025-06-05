#include "Enemy.hpp"

#include "../Game.hpp"
#include "Component/CircleComponent.hpp"
#include "Component/NavComponent.hpp"
#include "Component/SpriteComponent.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include <algorithm>

namespace EngineG {

Enemy::Enemy(class Game* game) : Actor(game) {
    // Add to enemy vector
    game->GetEnemies().emplace_back(this);

    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Airplane.png"));
    // Set position at start tile
    SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
    // Setup a nav component at the start tile
    NavComponent* nc = new NavComponent(this);
    nc->SetForwardSpeed(150.0f);
    nc->StartPath(GetGame()->GetGrid()->GetStartTile());
    // Setup a circle for collision
    mCircle = new CircleComponent(this);
    mCircle->SetRadius(25.0f);
}

Enemy::~Enemy() {
    // Remove from enemy vector
    auto iter = std::find(GetGame()->GetEnemies().begin(), GetGame()->GetEnemies().end(), this);
    GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    // Am I near the end tile?
    Vector2 diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
    if (Math::NearZero(diff.Length(), 10.0f)) {
        SetState(EDead);
    }
}

}  // namespace EngineG
