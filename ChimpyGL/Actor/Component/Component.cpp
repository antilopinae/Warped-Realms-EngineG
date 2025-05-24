#include "Component.hpp"

#include "Actor.hpp"

namespace EngineG
{

Component::Component(class Actor* owner, int updateOrder) : mOwner(owner), mUpdateOrder(updateOrder)
{
  // Add to actor's vector of components
  mOwner->AddComponent(this);
}

Component::~Component()
{
  mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {}


}    // namespace EngineG
