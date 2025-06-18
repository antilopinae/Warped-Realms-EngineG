#pragma once

#include "WasmHostShared.hpp"
#include <cstdint>
#include <string>

namespace EngineG {

class Component {
public:
    // (the lower the update order, the earlier the component updates)
    Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();
    virtual void Update(float deltaTime);

    // Process input for this component
    virtual void ProcessInput(const uint8_t* keyState) {}

    int GetUpdateOrder() const { return mUpdateOrder; }

    bool AttachWasmScriptToSelf(const std::string& scriptSlotNameOnThisComponent, const std::string& wasmModulePath);
    bool DetachWasmScriptFromSelf(const std::string& scriptSlotNameOnThisComponent);

    virtual std::vector<Wasm::HostFunctionBinding> GetHostBindingsForWasmSlot(const std::string& slotName) { return {}; }

protected:
    // Owning actor
    class Actor* mOwner;
    // Update order of component
    int mUpdateOrder;

protected:
    bool SendMessageToMyWasmScript(const std::string& scriptSlotNameOnThisComponent,
                                   const std::string& fromIdHintForWasm,
                                   const std::string& messageContent);

    std::string GetWasmEntityIdForOwnerActor() const;
};

}  // namespace EngineG
