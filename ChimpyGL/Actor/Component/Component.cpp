#include "Component.hpp"

#include "../../WasmIntegrationService.hpp"
#include "Actor.hpp"
#include "Game.hpp"

namespace EngineG {

Component::Component(class Actor* owner, int updateOrder) : mOwner(owner), mUpdateOrder(updateOrder) {
    mOwner->AddComponent(this);
}

Component::~Component() {
    mOwner->RemoveComponent(this);
    // TODO: Рассмотреть автоматическую выгрузку всех WASM скриптов, привязанных к этому компоненту,
    // Например: for (const auto& slot : mKnownWasmSlots) { DetachWasmScriptFromSelf(slot); }
}

void Component::Update(float deltaTime) {
}

std::string Component::GetWasmEntityIdForOwnerActor() const {
    if (!mOwner) return "COMPONENT_WITHOUT_OWNER";
    return mOwner->GetUniqueActorIdAsString();
}

bool Component::AttachWasmScriptToSelf(const std::string& scriptSlotNameOnThisComponent, const std::string& wasmModulePath) {
    if (!mOwner) {
        Wasm::LogMessage("EngineG::Component::AttachWasmScriptToSelf ERROR: Component has no owner actor.");
        return false;
    }
    std::string ownerActorId = GetWasmEntityIdForOwnerActor();
    std::vector<Wasm::HostFunctionBinding> specificBindings = GetHostBindingsForWasmSlot(scriptSlotNameOnThisComponent);

    return mOwner->GetGame()->GetWasmService().AttachWasmComponent(
        ownerActorId, scriptSlotNameOnThisComponent, wasmModulePath, specificBindings);
}

bool Component::DetachWasmScriptFromSelf(const std::string& scriptSlotNameOnThisComponent) {
    if (!mOwner) return false;
    std::string ownerActorId = GetWasmEntityIdForOwnerActor();
    return mOwner->GetGame()->GetWasmService().DetachWasmComponent(ownerActorId, scriptSlotNameOnThisComponent);
}

bool Component::SendMessageToMyWasmScript(const std::string& scriptSlotNameOnThisComponent,
                                          const std::string& fromIdHintForWasm,
                                          const std::string& messageContent) {
    if (!mOwner) return false;
    std::string ownerActorId = GetWasmEntityIdForOwnerActor();
    return mOwner->GetGame()->GetWasmService().SendMessageToWasm(
        fromIdHintForWasm, ownerActorId, scriptSlotNameOnThisComponent, messageContent);
}

}  // namespace EngineG
