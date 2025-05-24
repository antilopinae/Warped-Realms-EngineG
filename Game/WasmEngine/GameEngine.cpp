#include "GameEngine.hpp"
#include "HostApi.hpp"    // Для LogMessage
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace WasmHost {

bool GameEngine::ReadFileToVector(const std::string& path, std::vector<byte_t>& outBytes) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        LogMessage("GameEngine ERROR: Failed to open WASM file: " + path);
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    outBytes.resize(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(outBytes.data()), size)) {
        LogMessage("GameEngine ERROR: Failed to read WASM file: " + path);
        file.close();
        return false;
    }
    file.close();
    return true;
}


GameEngine::GameEngine() {
    mWasmEngine = wasm_engine_new();
    if (!mWasmEngine) {
        throw std::runtime_error("Failed to create Wasmtime engine.");
    }
}

GameEngine::~GameEngine() {
    for (auto it = mLoadedComponents.rbegin(); it != mLoadedComponents.rend(); ++it) {
         if (it->second) {
             if(it->second->IsInitialized()){
                it->second->Terminate();
             }
        }
    }
    mLoadedComponents.clear();

    if (mWasmEngine) wasm_engine_delete(mWasmEngine);
}

std::string GameEngine::GenerateComponentId(const std::string& entityName, const std::string& slotName, const std::string& path) {
    std::string shortPath = path;
    size_t lastSlash = shortPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) shortPath = shortPath.substr(lastSlash + 1);
    size_t dotPos = shortPath.rfind(".wasm");
    if (dotPos != std::string::npos) shortPath.erase(dotPos, 5);
    return entityName + "." + slotName + "[" + shortPath + "]";
}

bool GameEngine::AttachComponent(const std::string& entityName, const std::string& slotName, const std::string& wasmModulePath) {

    if (mEntities.find(entityName) == mEntities.end()) {
        mEntities[entityName] = Entity{entityName, {}};
    }

    std::string new_component_id = GenerateComponentId(entityName, slotName, wasmModulePath);
    auto& entity_slots = mEntities[entityName].mComponentSlots;

    if (entity_slots.count(slotName)) {
        std::string old_component_id = entity_slots[slotName];
        LogMessage("GameEngine: Slot '" + slotName + "' occupied by '" + old_component_id + "'. Unloading it.");
        UnloadComponentFromSlot(entityName, slotName); // Это обеспечит вызов Terminate и удаление
    }

    auto new_component_ptr = std::make_unique<WasmComponent>(new_component_id, wasmModulePath, entityName, slotName, this, mWasmEngine);

    if (!new_component_ptr->Initialize()) {
        LogMessage("GameEngine ERROR: Failed to initialize new component " + new_component_id);
        return false;
    }

    mLoadedComponents[new_component_id] = std::move(new_component_ptr);
    entity_slots[slotName] = new_component_id;

    return true;
}

void GameEngine::UnloadComponentInternal(const std::string& componentId) {
    auto it = mLoadedComponents.find(componentId);
    if (it == mLoadedComponents.end()) {
        return;
    }

    if (it->second) {
        if(it->second->IsInitialized()){
            it->second->Terminate();
        }
    }
    mLoadedComponents.erase(it);
}

bool GameEngine::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
    auto entity_it = mEntities.find(entityName);
    if (entity_it == mEntities.end()) {
        LogMessage("GameEngine WARNING: Entity '" + entityName + "' not found for unload.");
        return false;
    }

    auto& entity_slots = entity_it->second.mComponentSlots;
    auto slot_it = entity_slots.find(slotName);
    if (slot_it == entity_slots.end()) {
        LogMessage("GameEngine WARNING: Slot '" + slotName + "' not found on entity '" + entityName + "'.");
        return false;
    }

    std::string component_id_to_unload = slot_it->second;
    entity_slots.erase(slot_it);

    UnloadComponentInternal(component_id_to_unload);
    return true;
}

void GameEngine::SendMessage(const std::string& fromComponentId, const std::string& toEntityAndSlotName, const std::string& messageContent) {
    mMessageQueue.push_back({fromComponentId, toEntityAndSlotName, messageContent});
}

void GameEngine::ProcessMessageQueue() {
    if (mMessageQueue.empty()) return;

    for (const auto& msg : mMessageQueue) {
        size_t dot_pos = msg.mToEntitySlot.find('.');
        if (dot_pos == std::string::npos) {
            LogMessage("GameEngine WARNING: Invalid 'to' address (no dot): " + msg.mToEntitySlot);
            continue;
        }
        std::string target_entity_name = msg.mToEntitySlot.substr(0, dot_pos);
        std::string target_slot_name = msg.mToEntitySlot.substr(dot_pos + 1);

        auto entity_it = mEntities.find(target_entity_name);
        if (entity_it == mEntities.end()) continue;

        auto slot_it = entity_it->second.mComponentSlots.find(target_slot_name);
        if (slot_it == entity_it->second.mComponentSlots.end()) continue;

        std::string target_component_id = slot_it->second;
        auto component_it = mLoadedComponents.find(target_component_id);
        if (component_it != mLoadedComponents.end() && component_it->second) {
            component_it->second->mInbox.push_back(msg);
        }
    }
    mMessageQueue.clear();
}

void GameEngine::DeliverInboxMessages() {
    for (auto const& [id, component_ptr] : mLoadedComponents) {
        if (component_ptr && component_ptr->IsInitialized() && !component_ptr->mInbox.empty()) {
            for (const auto& msg_payload : component_ptr->mInbox) {
                component_ptr->DeliverMessage(msg_payload.mFromId, msg_payload.mContent);
            }
            component_ptr->mInbox.clear();
        }
    }
}

void GameEngine::Update() {
    for (auto const& [id, component_ptr] : mLoadedComponents) {
        if (component_ptr && component_ptr->IsInitialized()) {
             component_ptr->Update();
        }
    }
    ProcessMessageQueue();
    DeliverInboxMessages();
}

void GameEngine::HandleWasmTrap(const std::string& funcName, wasm_trap_t* trap, const std::string& componentId) {
    wasm_message_t trap_message;
    wasm_trap_message(trap, &trap_message);
    std::string trap_message_str(trap_message.data, trap_message.size);

    wasm_byte_vec_delete(&trap_message);
    wasm_trap_delete(trap);
}

} // namespace WasmHost