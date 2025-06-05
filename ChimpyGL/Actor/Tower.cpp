#include "Tower.hpp"

#include "../Game.hpp"
#include "Bullet.hpp"
#include "Component/MoveComponent.hpp"
#include "Component/SpriteComponent.hpp"
#include "Enemy.hpp"

namespace EngineG {

Tower::Tower(class Game* game) : Actor(game) {
    SpriteComponent* sc = new SpriteComponent(this, 200);
    sc->SetTexture(game->GetTexture("Tower.png"));

    mMove = new MoveComponent(this);
    // mMove->SetAngularSpeed(Math::Pi);

    mNextAttack = AttackTime;
}

void Tower::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    mNextAttack -= deltaTime;
    if (mNextAttack <= 0.0f) {
        Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
        if (e != nullptr) {
            // Vector from me to enemy
            Vector2 dir = e->GetPosition() - GetPosition();
            float dist = dir.Length();
            if (dist < AttackRange) {
                // Rotate to face enemy
                SetRotation(Math::Atan2(-dir.y, dir.x));
                // Spawn bullet at tower position facing enemy
                Bullet* b = new Bullet(GetGame());
                b->SetPosition(GetPosition());
                b->SetRotation(GetRotation());
            }
        }
        mNextAttack += AttackTime;
    }
}

}  // namespace EngineG
