#pragma once

#include <cstdint>

namespace EngineG{

class Component
{
public:
    // (the lower the update order, the earlier the component updates)
    Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();
    virtual void Update(float deltaTime);

    // Process input for this component
    virtual void ProcessInput(const uint8_t* keyState) {}

    int GetUpdateOrder() const { return mUpdateOrder; }
protected:
    // Owning actor
    class Actor* mOwner;
    // Update order of component
    int mUpdateOrder;
};


}
