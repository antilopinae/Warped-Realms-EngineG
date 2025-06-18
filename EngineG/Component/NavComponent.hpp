#pragma once

#include "../Math/Math.hpp"
using namespace EngineG::Math;

#include "MoveComponent.hpp"

namespace EngineG {

class NavComponent : public MoveComponent {
public:
    // Lower update order to update first
    NavComponent(class Actor* owner, int updateOrder = 10);
    void Update(float deltaTime) override;
    void StartPath(const class Tile* start);
    void TurnTo(const Vector2& pos);

private:
    const class Tile* mNextNode;
};

}  // namespace EngineG
