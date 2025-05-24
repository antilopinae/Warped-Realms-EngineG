#include "WasmEngine.hpp"

#include "WasmComponent.hpp"
#include "WasmHostApi.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace EngineG::Wasm {

WasmEngine::WasmEngine()
    : mEngine([] {
        wasmtime::Config config;
        config.consume_fuel(true);
        config.epoch_interruption(true);
        return wasmtime::Engine(std::move(config));
    }()) {
}

bool WasmEngine::ReadFileToVector(const std::string& path, std::vector<uint8_t>& outBytes) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        LogMessage("WasmEngine ERROR: Failed to open WASM file: " + path);
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size <= 0) {
        outBytes.clear();
        file.close();
        LogMessage(size == 0 ? "WasmEngine WARN: WASM file is empty: " + path
                             : "WasmEngine ERROR: Failed to get size of WASM file: " + path);
        return size == 0;
    }
    outBytes.resize(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(outBytes.data()), size)) {
        LogMessage("WasmEngine ERROR: Failed to read WASM file: " + path);
        file.close();
        return false;
    }
    file.close();
    return true;
}

std::string WasmEngine::GenerateComponentId(const std::string& entityName, const std::string& slotName, const std::string& path) {
    std::string shortPath = path;
    size_t lastSlash = shortPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        shortPath = shortPath.substr(lastSlash + 1);
    }
    size_t dotPos = shortPath.rfind(".wasm");
    if (dotPos != std::string::npos) {
        shortPath.erase(dotPos, 5);
    }
    return entityName + "." + slotName + "[" + shortPath + "]";
}

bool WasmEngine::AttachComponent(const std::string& entityName,
                                 const std::string& slotName,
                                 const std::string& wasmModulePath,
                                 const std::vector<HostFunctionBinding>& specificHostFunctions) {
    std::string new_component_id = GenerateComponentId(entityName, slotName, wasmModulePath);

    if (mEntities.find(entityName) == mEntities.end()) {
        mEntities[entityName] = WasmEntity{entityName, {}};
    }

    auto& entity_slots = mEntities.at(entityName).mComponentSlots;
    if (entity_slots.count(slotName)) {
        std::string old_component_id = entity_slots.at(slotName);
        LogMessage("WasmEngine: Slot '" + slotName + "' on entity '" + entityName + "' is occupied by '" + old_component_id +
                   "'. Unloading old component.");
        if (!UnloadComponentFromSlot(entityName, slotName)) {
            LogMessage("WasmEngine ERROR: Failed to unload existing component '" + old_component_id + "' from slot '" + slotName +
                       "'. Attach failed.");
            return false;
        }
    }

    try {
        auto new_component_ptr = std::make_unique<WasmComponent>(
            new_component_id, wasmModulePath, entityName, slotName, this, mEngine, specificHostFunctions);
        if (!new_component_ptr->Initialize()) {
            LogMessage("WasmEngine ERROR: Failed to initialize WasmComponent '" + new_component_id + "'.");
            return false;
        }
        mLoadedComponents[new_component_id] = std::move(new_component_ptr);
        entity_slots[slotName] = new_component_id;
        return true;
    } catch (const wasmtime::Error& e) {
        LogWasmException("AttachComponent: " + new_component_id, e, new_component_id);
        return false;
    } catch (const wasmtime::Trap& t) {
        LogWasmException("AttachComponent: " + new_component_id, t, new_component_id);
        return false;
    } catch (const std::exception& e) {
        LogWasmException("AttachComponent: " + new_component_id, e, new_component_id);
        return false;
    }
}

void WasmEngine::UnloadComponentInternal(const std::string& componentId) {
    auto it = mLoadedComponents.find(componentId);
    if (it == mLoadedComponents.end()) {
        LogMessage("WasmEngine WARN: UnloadComponentInternal - Component ID '" + componentId +
                   "' not found in loaded components.");
        return;
    }
    try {
        if (it->second && it->second->IsInitialized()) {
            it->second->Terminate();
        }
    } catch (const wasmtime::Error& e) {
        LogWasmException("UnloadComponentInternal - Terminate", e, componentId);
    } catch (const wasmtime::Trap& t) {
        LogWasmException("UnloadComponentInternal - Terminate", t, componentId);
    } catch (const std::exception& e) {
        LogWasmException("UnloadComponentInternal - Terminate", e, componentId);
    }
    mLoadedComponents.erase(it);
}

bool WasmEngine::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
    auto entity_it = mEntities.find(entityName);
    if (entity_it == mEntities.end()) {
        LogMessage("WasmEngine WARN: UnloadComponentFromSlot - Entity '" + entityName + "' not found.");
        return false;
    }

    auto& entity_slots = entity_it->second.mComponentSlots;
    auto slot_it = entity_slots.find(slotName);
    if (slot_it == entity_slots.end()) {
        LogMessage("WasmEngine INFO: UnloadComponentFromSlot - Slot '" + slotName + "' on entity '" + entityName +
                   "' is already empty.");
        return true;
    }

    std::string component_id_to_unload = slot_it->second;
    entity_slots.erase(slot_it);
    UnloadComponentInternal(component_id_to_unload);
    return true;
}

void WasmEngine::ProcessMessageQueue() {
    if (mMessageQueueInternal.empty()) return;
    std::vector<WasmMessagePayload> current_queue = std::move(mMessageQueueInternal);
    for (const auto& queued_msg : current_queue) {
        size_t dot_pos = queued_msg.mToId.find('.');
        if (dot_pos == std::string::npos) {
            LogMessage("WasmEngine WARN: Malformed ToId in message (missing '.'): '" + queued_msg.mToId + "'. Skipping.");
            continue;
        }
        std::string target_entity_name = queued_msg.mToId.substr(0, dot_pos);
        std::string target_slot_name = queued_msg.mToId.substr(dot_pos + 1);
        auto entity_it = mEntities.find(target_entity_name);
        if (entity_it == mEntities.end()) {
            LogMessage("WasmEngine WARN: Target entity '" + target_entity_name + "' not found for message. Skipping.");
            continue;
        }
        auto slot_it = entity_it->second.mComponentSlots.find(target_slot_name);
        if (slot_it == entity_it->second.mComponentSlots.end()) {
            LogMessage("WasmEngine WARN: Target slot '" + target_slot_name + "' on entity '" + target_entity_name +
                       "' not found. Skipping.");
            continue;
        }
        std::string target_component_id = slot_it->second;
        auto component_it = mLoadedComponents.find(target_component_id);
        if (component_it != mLoadedComponents.end() && component_it->second) {
            component_it->second->mInbox.push_back({queued_msg.mFromId, target_component_id, queued_msg.mContent});
        } else {
            LogMessage("WasmEngine WARN: Target component ID '" + target_component_id +
                       "' (from slot) not found in loaded components. Skipping message.");
        }
    }
}

void WasmEngine::DeliverInboxMessages() {
    if (mLoadedComponents.empty()) return;
    for (auto const& [id, component_ptr] : mLoadedComponents) {
        if (component_ptr && component_ptr->IsInitialized() && !component_ptr->mInbox.empty()) {
            std::vector<WasmMessagePayload> inboxCopy = std::move(component_ptr->mInbox);
            for (const auto& msg_payload : inboxCopy) {
                try {
                    if (!component_ptr->DeliverMessage(msg_payload.mFromId, msg_payload.mContent)) {
                        LogMessage("WasmEngine WARN: Component '" + id + "' DeliverMessage returned false for message from '" +
                                   msg_payload.mFromId + "'.");
                    }
                } catch (const wasmtime::Error& e) {
                    LogWasmException("DeliverInboxMessages to " + id, e, id);
                } catch (const wasmtime::Trap& t) {
                    LogWasmException("DeliverInboxMessages to " + id, t, id);
                } catch (const std::exception& e) {
                    LogWasmException("DeliverInboxMessages to " + id, e, id);
                }
            }
        }
    }
}

void WasmEngine::ScheduleDeferredWasmUpdate(const std::string& componentId) {
    mDeferredWasmUpdates.push_back({componentId});
}

void WasmEngine::ProcessDeferredWasmUpdates() {
    if (mDeferredWasmUpdates.empty()) return;
    std::vector<WasmDeferredUpdate> updatesToProcess = std::move(mDeferredWasmUpdates);
    for (const auto& call : updatesToProcess) {
        auto component_it = mLoadedComponents.find(call.targetComponentId);
        if (component_it != mLoadedComponents.end() && component_it->second && component_it->second->IsInitialized()) {
            try {
                if (!component_it->second->TriggerWasmUpdate()) {
                    LogMessage("WasmEngine WARN: TriggerWasmUpdate for component '" + call.targetComponentId +
                               "' returned false.");
                }
            } catch (const wasmtime::Error& e) {
                LogWasmException("ProcessDeferredWasmUpdates - TriggerWasmUpdate", e, call.targetComponentId);
            } catch (const wasmtime::Trap& t) {
                LogWasmException("ProcessDeferredWasmUpdates - TriggerWasmUpdate", t, call.targetComponentId);
            } catch (const std::exception& e) {
                LogWasmException("ProcessDeferredWasmUpdates - TriggerWasmUpdate", e, call.targetComponentId);
            }
        } else {
            LogMessage("WasmEngine WARN: Deferred WASM update for component '" + call.targetComponentId +
                       "' skipped (not found, null, or not initialized).");
        }
    }
}

void WasmEngine::Update() {
    for (auto const& [id, component_ptr] : mLoadedComponents) {
        if (component_ptr && component_ptr->IsInitialized()) {
            ScheduleDeferredWasmUpdate(id);
        }
    }
    ProcessMessageQueue();
    DeliverInboxMessages();
    ProcessDeferredWasmUpdates();
}

void WasmEngine::SendMessage(const std::string& fromComponentId,
                             const std::string& toEntityAndSlotName,
                             const std::string& messageContent) {
    mMessageQueueInternal.push_back({fromComponentId, toEntityAndSlotName, messageContent});
}

void WasmEngine::LogWasmException(const std::string& operationName, const std::exception& e, const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "WasmEngine EXCEPTION (std): Component '" << componentId << "' Operation '" << operationName << "'. "
             << "Exception: '" << e.what() << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

void WasmEngine::LogWasmException(const std::string& operationName, const wasmtime::Error& e, const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "WasmEngine EXCEPTION (wasmtime::Error): Component '" << componentId << "' Operation '" << operationName << "'. "
             << "Error: '" << e.message() << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

void WasmEngine::LogWasmException(const std::string& operationName, const wasmtime::Trap& e, const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "WasmEngine EXCEPTION (wasmtime::Trap): Component '" << componentId << "' Operation '" << operationName << "'. "
             << "Trap: '" << e.message() << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

void WasmEngine::LogWasmException(const std::string& operationName, const std::string& e, const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "WasmEngine EXCEPTION (wasmtime::Trap): Component '" << componentId << "' Operation '" << operationName << "'. "
             << "Trap: '" << e << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

WasmEngine::~WasmEngine() {
    for (auto it = mLoadedComponents.rbegin(); it != mLoadedComponents.rend(); ++it) {
        if (it->second) {
            try {
                it->second->Terminate();
            } catch (const wasmtime::Error& e) {
                LogWasmException("WasmEngine Dtor - Terminate", e, it->first);
            } catch (const wasmtime::Trap& t) {
                LogWasmException("WasmEngine Dtor - Terminate", t, it->first);
            } catch (const std::exception& e) {
                LogWasmException("WasmEngine Dtor - Terminate", e, it->first);
            }
        }
    }
    mLoadedComponents.clear();
}

}  // namespace EngineG::Wasm