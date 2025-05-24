#include "GameEngine.hpp"
#include "WasmComponent.hpp"
#include "HostApi.hpp" // For WasmHost::LogMessage

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream> // For LogWasmException

namespace WasmHost {

GameEngine::GameEngine()
    : mEngine([] {
          wasmtime::Config config;
          config.consume_fuel(true);
          config.epoch_interruption(true);
          return wasmtime::Engine(std::move(config));
      }()),
      mLinker(mEngine) {
    LogMessage("GameEngine: Constructor - Wasmtime engine created.");

    try {
        mLinker.func_wrap("env", "engine_log_wasm",
            [](wasmtime::Caller caller, uint32_t ptr, uint32_t len) {
                // Get context from caller
                wasmtime::Store::Context store_context = caller.context();

                // Get std::any& from context
                std::any& host_data_any = store_context.get_data(); // As per your hint

                WasmComponent* component = nullptr;
                try {
                    component = std::any_cast<WasmComponent*>(host_data_any);
                } catch (const std::bad_any_cast& e) {
                    std::cerr << "CRITICAL_WASM_LOG_ERROR: Failed to cast host data: " << e.what() << std::endl;
                    throw wasmtime::Trap("env.engine_log_wasm: host data type mismatch.");
                }

                if (!component) {
                    std::cerr << "CRITICAL_WASM_LOG_ERROR: env.engine_log_wasm called with null component context after cast." << std::endl;
                    throw wasmtime::Trap("env.engine_log_wasm: null component context after cast.");
                }
                try {
                    std::string msg_str = component->ReadStringFromGuestMemory(ptr, len);
                    LogMessage("WASM_LOG[" + component->GetId() + "]: " + msg_str);
                } catch (const std::exception& e) { // Catch errors from ReadStringFromGuestMemory
                    LogMessage("WASM_LOG_ERROR[" + component->GetId() + "]: Error reading log message from guest: " + e.what());
                    throw wasmtime::Trap("Failed to read log message from guest memory: " + std::string(e.what()));
                }
            }).unwrap();

        mLinker.func_wrap("env", "emscripten_notify_memory_growth",
            [](int32_t /*memory_index*/) {
            }).unwrap();

    } catch (const wasmtime::Error& e) { // Catch errors from func_wrap or unwrap
        LogMessage("GameEngine FATAL: Failed to define host functions: " + std::string(e.message()));
        std::cerr << "GameEngine FATAL: Failed to define host functions: " << e.message() << std::endl;
        throw; // Re-throw to indicate critical setup failure
    }
    LogMessage("GameEngine: Linker created and host functions defined.");
}

bool GameEngine::ReadFileToVector(const std::string& path, std::vector<uint8_t>& outBytes) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        LogMessage("GameEngine ERROR: Failed to open WASM file: " + path);
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size <= 0) {
        outBytes.clear();
        file.close();
        LogMessage(size == 0 ? "GameEngine WARN: WASM file is empty: " + path
                             : "GameEngine ERROR: Failed to get size of WASM file: " + path);
        return size == 0; // True if empty (valid), false if error
    }
    outBytes.resize(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(outBytes.data()), size)) {
        LogMessage("GameEngine ERROR: Failed to read WASM file: " + path);
        file.close();
        return false;
    }
    file.close();
    return true;
}

std::string GameEngine::GenerateComponentId(const std::string& entityName, const std::string& slotName, const std::string& path) {
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


bool GameEngine::AttachComponent(const std::string& entityName, const std::string& slotName, const std::string& wasmModulePath) {
    std::string new_component_id = GenerateComponentId(entityName, slotName, wasmModulePath);
    LogMessage("GameEngine: Attempting to attach component: " + new_component_id + " from " + wasmModulePath);

    if (mEntities.find(entityName) == mEntities.end()) {
        mEntities[entityName] = Entity{entityName, {}};
        LogMessage("GameEngine: Created new entity: " + entityName);
    }

    auto& entity_slots = mEntities.at(entityName).mComponentSlots;
    if (entity_slots.count(slotName)) {
        std::string old_component_id = entity_slots.at(slotName);
        LogMessage("GameEngine: Slot '" + slotName + "' on entity '" + entityName + "' is occupied by '" + old_component_id + "'. Unloading old component.");
        if (!UnloadComponentFromSlot(entityName, slotName)) {
            LogMessage("GameEngine ERROR: Failed to unload existing component '" + old_component_id + "' from slot '" + slotName + "'. Attach failed.");
            return false;
        }
    }

    try {
        auto new_component_ptr = std::make_unique<WasmComponent>(new_component_id, wasmModulePath, entityName, slotName, this, mEngine);
        if (!new_component_ptr->Initialize()) { // Initialize can throw or return false
            LogMessage("GameEngine ERROR: Failed to initialize WasmComponent '" + new_component_id + "'.");
            return false;
        }
        mLoadedComponents[new_component_id] = std::move(new_component_ptr);
        entity_slots[slotName] = new_component_id;
        LogMessage("GameEngine: Successfully attached and initialized component: " + new_component_id);
        return true;
    } catch (const wasmtime::Error& e) {
        LogWasmException("AttachComponent: " + new_component_id, e, new_component_id);
        return false;
    } catch (const wasmtime::Trap& t) {
        LogWasmException("AttachComponent: " + new_component_id, t, new_component_id);
        return false;
    } catch (const std::exception& e) { // Catch other standard exceptions like std::runtime_error
        LogWasmException("AttachComponent: " + new_component_id, e, new_component_id);
        return false;
    }
}

void GameEngine::UnloadComponentInternal(const std::string& componentId) {
  auto it = mLoadedComponents.find(componentId);
  if (it == mLoadedComponents.end()) {
    LogMessage("GameEngine WARN: UnloadComponentInternal - Component ID '" + componentId + "' not found in loaded components.");
    return;
  }
  LogMessage("GameEngine: Unloading component internal: " + componentId);
  try {
    if (it->second && it->second->IsInitialized()) {
      it->second->Terminate(); // Terminate can throw
    }
  } catch (const wasmtime::Error& e) {
    LogWasmException("UnloadComponentInternal - Terminate", e, componentId);
  } catch (const wasmtime::Trap& t) {
    LogWasmException("UnloadComponentInternal - Terminate", t, componentId);
  } catch (const std::exception& e) { // Fallback
    LogWasmException("UnloadComponentInternal - Terminate", e, componentId);
  }
  mLoadedComponents.erase(it);
  LogMessage("GameEngine: Component '" + componentId + "' erased from loaded components.");
}

// ... (UnloadComponentFromSlot, SendMessage, ProcessMessageQueue, DeliverInboxMessages, ScheduleDeferredWasmUpdate, ProcessDeferredWasmUpdates, Update - largely unchanged but ensure they call LogWasmException correctly on caught exceptions)
bool GameEngine::UnloadComponentFromSlot(const std::string& entityName, const std::string& slotName) {
  LogMessage("GameEngine: Attempting to unload component from Entity: '" + entityName + "', Slot: '" + slotName + "'.");
  auto entity_it = mEntities.find(entityName);
  if (entity_it == mEntities.end()) {
    LogMessage("GameEngine WARN: UnloadComponentFromSlot - Entity '" + entityName + "' not found.");
    return false;
  }

  auto& entity_slots = entity_it->second.mComponentSlots;
  auto slot_it = entity_slots.find(slotName);
  if (slot_it == entity_slots.end()) {
    LogMessage("GameEngine INFO: UnloadComponentFromSlot - Slot '" + slotName + "' on entity '" + entityName + "' is already empty.");
    return true;
  }

  std::string component_id_to_unload = slot_it->second;
  LogMessage("GameEngine: Found component '" + component_id_to_unload + "' in slot. Removing from slot map.");
  entity_slots.erase(slot_it);
  UnloadComponentInternal(component_id_to_unload);
  return true;
}

void GameEngine::ProcessMessageQueue() {
    if (mMessageQueueInternal.empty()) return;
    std::vector<MessagePayload> current_queue = std::move(mMessageQueueInternal);
    LogMessage("GameEngine: Processing " + std::to_string(current_queue.size()) + " messages from internal queue.");
    for (const auto& queued_msg : current_queue) {
        size_t dot_pos = queued_msg.mToId.find('.');
        if (dot_pos == std::string::npos) {
            LogMessage("GameEngine WARN: Malformed ToId in message (missing '.'): '" + queued_msg.mToId + "'. Skipping.");
            continue;
        }
        std::string target_entity_name = queued_msg.mToId.substr(0, dot_pos);
        std::string target_slot_name = queued_msg.mToId.substr(dot_pos + 1);
        auto entity_it = mEntities.find(target_entity_name);
        if (entity_it == mEntities.end()) {
            LogMessage("GameEngine WARN: Target entity '" + target_entity_name + "' not found for message. Skipping.");
            continue;
        }
        auto slot_it = entity_it->second.mComponentSlots.find(target_slot_name);
        if (slot_it == entity_it->second.mComponentSlots.end()) {
            LogMessage("GameEngine WARN: Target slot '" + target_slot_name + "' on entity '" + target_entity_name + "' not found. Skipping.");
            continue;
        }
        std::string target_component_id = slot_it->second;
        auto component_it = mLoadedComponents.find(target_component_id);
        if (component_it != mLoadedComponents.end() && component_it->second) {
            LogMessage("GameEngine: Adding message to inbox of component '" + target_component_id + "'.");
            component_it->second->mInbox.push_back({queued_msg.mFromId, target_component_id, queued_msg.mContent});
        } else {
            LogMessage("GameEngine WARN: Target component ID '" + target_component_id + "' (from slot) not found in loaded components. Skipping message.");
        }
    }
}

void GameEngine::DeliverInboxMessages() {
    if (mLoadedComponents.empty()) return;
    for (auto const& [id, component_ptr] : mLoadedComponents) {
        if (component_ptr && component_ptr->IsInitialized() && !component_ptr->mInbox.empty()) {
            std::vector<MessagePayload> inboxCopy = std::move(component_ptr->mInbox);
            LogMessage("GameEngine: Component '" + id + "' has " + std::to_string(inboxCopy.size()) + " messages to process.");
            for (const auto& msg_payload : inboxCopy) {
                try {
                    LogMessage("GameEngine: Delivering message from '" + msg_payload.mFromId + "' to '" + id + "' content: '" + msg_payload.mContent + "'");
                    if (!component_ptr->DeliverMessage(msg_payload.mFromId, msg_payload.mContent)) {
                        LogMessage("GameEngine WARN: Component '" + id + "' DeliverMessage returned false for message from '" + msg_payload.mFromId + "'.");
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

void GameEngine::ScheduleDeferredWasmUpdate(const std::string& componentId) {
    m_deferredWasmUpdates.push_back({componentId});
}

void GameEngine::ProcessDeferredWasmUpdates() {
  if (m_deferredWasmUpdates.empty()) return;
  std::vector<DeferredWasmUpdate> updatesToProcess = std::move(m_deferredWasmUpdates);
  for (const auto& call : updatesToProcess) {
    auto component_it = mLoadedComponents.find(call.targetComponentId);
    if (component_it != mLoadedComponents.end() && component_it->second && component_it->second->IsInitialized()) {
      try {
        if (!component_it->second->TriggerWasmUpdate()) {
             LogMessage("GameEngine WARN: TriggerWasmUpdate for component '" + call.targetComponentId + "' returned false.");
        }
      } catch (const wasmtime::Error& e) {
          LogWasmException("ProcessDeferredWasmUpdates - TriggerWasmUpdate", e, call.targetComponentId);
      } catch (const wasmtime::Trap& t) {
          LogWasmException("ProcessDeferredWasmUpdates - TriggerWasmUpdate", t, call.targetComponentId);
      } catch (const std::exception& e) {
          LogWasmException("ProcessDeferredWasmUpdates - TriggerWasmUpdate", e, call.targetComponentId);
      }
    } else {
      LogMessage("GameEngine WARN: Deferred WASM update for component '" + call.targetComponentId + "' skipped (not found, null, or not initialized).");
    }
  }
}

void GameEngine::Update() {
    for (auto const& [id, component_ptr] : mLoadedComponents) {
      if (component_ptr && component_ptr->IsInitialized()) {
        ScheduleDeferredWasmUpdate(id);
      }
    }
    ProcessMessageQueue();
    DeliverInboxMessages();
    ProcessDeferredWasmUpdates();
}

void GameEngine::SendMessage(const std::string& fromComponentId, const std::string& toEntityAndSlotName, const std::string& messageContent) {
  LogMessage("GameEngine: Queuing message from '" + fromComponentId + "' to '" + toEntityAndSlotName + "' content: '" + messageContent + "'");
  mMessageQueueInternal.push_back({fromComponentId, toEntityAndSlotName, messageContent});
}

void GameEngine::LogWasmException(const std::string& operationName,
                                  const std::exception& e,
                                  const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "GameEngine EXCEPTION (std): Component '" << componentId
             << "' Operation '" << operationName << "'. "
             << "Exception: '" << e.what() << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

void GameEngine::LogWasmException(const std::string& operationName,
                                  const wasmtime::Error& e,
                                  const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "GameEngine EXCEPTION (wasmtime::Error): Component '" << componentId
             << "' Operation '" << operationName << "'. "
             << "Error: '" << e.message() << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

void GameEngine::LogWasmException(const std::string& operationName,
                                  const wasmtime::Trap& e,
                                  const std::string& componentId) {
    std::ostringstream errorLog;
    errorLog << "GameEngine EXCEPTION (wasmtime::Trap): Component '" << componentId
             << "' Operation '" << operationName << "'. "
             << "Trap: '" << e.message() << "'.";
    LogMessage(errorLog.str());
    std::cerr << errorLog.str() << std::endl;
}

GameEngine::~GameEngine() {
    LogMessage("GameEngine: Destructor - Clearing components...");
    // Unload in reverse order of attachment dependencies (if any, though not explicit here)
    // or simply iterate and terminate.
    // The mLoadedComponents map will clear unique_ptrs, calling WasmComponent destructors.
    for (auto it = mLoadedComponents.rbegin(); it != mLoadedComponents.rend(); ++it) {
        if (it->second) {
            try {
                it->second->Terminate();
            } catch (const wasmtime::Error& e) { // Catch specific wasmtime errors
                LogWasmException("GameEngine Dtor - Terminate", e, it->first);
            } catch (const wasmtime::Trap& t) {
                LogWasmException("GameEngine Dtor - Terminate", t, it->first);
            } catch (const std::exception& e) { // Catch standard exceptions
                LogWasmException("GameEngine Dtor - Terminate", e, it->first);
            }
        }
    }
    mLoadedComponents.clear();
    LogMessage("GameEngine: Destructor finished. Wasmtime objects will self-clean.");
}

} // namespace WasmHost