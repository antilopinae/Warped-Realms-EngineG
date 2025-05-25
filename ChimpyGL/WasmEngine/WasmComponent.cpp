#include "WasmComponent.hpp"

#include "WasmHostApi.hpp"
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace EngineG::Wasm {

WasmComponent::WasmComponent(const std::string& componentId,
                             const std::string& wasmPath,
                             const std::string& entityName,
                             const std::string& slotName,
                             WasmEngine* ownerEngine,
                             wasmtime::Engine& engine,
                             const std::vector<HostFunctionBinding>& specificHostFunctions)
    : mId(componentId)
    , mPath(wasmPath)
    , mEntityName(entityName)
    , mSlotName(slotName)
    , mOwnerEngine(ownerEngine)
    , mEngineRef(engine)
    , mSpecificHostFunctions(specificHostFunctions)
    , mIsInitialized(false) {
    if (!mOwnerEngine) {
        throw std::runtime_error("WasmComponent [" + mId + "]: OwnerGameEngine cannot be null.");
    }
}

WasmComponent::~WasmComponent() {
    if (mIsInitialized) {
        try {
            Terminate();
        } catch (const wasmtime::Error& e) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("WasmComponent Dtor - Terminate", e, mId);
        } catch (const wasmtime::Trap& t) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("WasmComponent Dtor - Terminate", t, mId);
        } catch (const std::exception& e) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("WasmComponent Dtor - Terminate", e, mId);
        }
    }
}

bool WasmComponent::Initialize() {
    if (mIsInitialized) {
        LogMessage("WasmComponent [" + mId + "]: Already initialized.");
        return true;
    }
    try {
        mStoreOpt.emplace(mEngineRef);
        mStoreOpt->context().set_data(std::any(this));

        auto fuel_res = mStoreOpt->context().set_fuel(100'000'000);
        if (!fuel_res) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("Initialize set_fuel", fuel_res.err(), mId);
        }
        mStoreOpt->context().set_epoch_deadline(1);

        if (!LoadAndInstantiateModule()) {
            LogMessage("WasmComponent [" + mId + "] ERROR: LoadAndInstantiateModule failed during Initialize.");
            mStoreOpt.reset();
            return false;
        }
        mIsInitialized = true;

        if (mInitializeFuncOpt) {
            mInitializeFuncOpt->call(mStoreOpt->context(), {}).unwrap();
        } else {
            LogMessage("WasmComponent [" + mId + "]: No WASM 'initialize' function found/exported.");
        }
        return true;
    } catch (const wasmtime::Error& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM Initialize (Error)", e, mId);
    } catch (const wasmtime::Trap& t) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM Initialize (Trap)", t, mId);
    } catch (const std::exception& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM Initialize (StdExc)", e, mId);
    }
    mStoreOpt.reset();
    mInstanceOpt.reset();
    mMemoryOpt.reset();
    return false;
}

bool WasmComponent::LoadAndInstantiateModule() {
    if (!mOwnerEngine->ReadFileToVector(mPath, mWasmBinaryData)) {
        return false;
    }
    if (mWasmBinaryData.empty()) {
        return false;
    }

    wasmtime::Result<wasmtime::Module, wasmtime::Error> module_compile_res =
        wasmtime::Module::compile(mEngineRef, mWasmBinaryData);
    if (!module_compile_res) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("Module::compile", module_compile_res.err(), mId);
        return false;
    }
    wasmtime::Module module = std::move(module_compile_res.unwrap());

    if (!mStoreOpt.has_value()) {
        return false;
    }

    wasmtime::Linker local_linker(mEngineRef);

    bool engine_log_is_specific = false;
    bool notify_growth_is_specific = false;
    for (const auto& binding : mSpecificHostFunctions) {
        if (binding.import_module_name == "env" && binding.import_function_name == "engine_log_wasm")
            engine_log_is_specific = true;
        if (binding.import_module_name == "env" && binding.import_function_name == "emscripten_notify_memory_growth")
            notify_growth_is_specific = true;
    }

    if (!engine_log_is_specific) {
        local_linker
            .func_wrap("env",
                       "engine_log_wasm",
                       [this](wasmtime::Caller caller, int32_t ptr, int32_t len) -> void {
                           if (this != std::any_cast<WasmComponent*>(caller.context().get_data())) {
                               LogMessage("Critical Ctx mismatch! this: " + std::to_string(reinterpret_cast<uintptr_t>(this)) +
                                          ", context_data: " +
                                          std::to_string(reinterpret_cast<uintptr_t>(
                                              std::any_cast<WasmComponent*>(caller.context().get_data()))));
                               throw wasmtime::Trap("Ctx mismatch in engine_log_wasm");
                           }
                           uint32_t u_ptr = static_cast<uint32_t>(ptr);
                           uint32_t u_len = static_cast<uint32_t>(len);
                           std::vector<uint8_t> bytes = this->ReadBytesFromGuestMemory(u_ptr, u_len);
                           LogMessage("WASM_LOG_COMMON[" + this->GetId() + "]: " + std::string(bytes.begin(), bytes.end()));
                       })
            .unwrap();
    }

    if (!notify_growth_is_specific) {
        local_linker.func_wrap("env", "emscripten_notify_memory_growth", [](int32_t /*mem_idx*/) { /*noop*/ }).unwrap();
    }

    for (const auto& binding : mSpecificHostFunctions) {
        if (binding.import_module_name == "env") {
        }
        LogMessage("WasmComponent [" + mId + "]: Defining specific binding for local Linker: " + binding.import_module_name +
                   "." + binding.import_function_name);

        std::visit(
            [&local_linker, this_comp = this, &binding_ref = binding](const auto& func_impl_from_variant) {
                using CurrentStdFuncInVariant = std::decay_t<decltype(func_impl_from_variant)>;
                const std::string& mod_name = binding_ref.import_module_name;
                const std::string& func_name = binding_ref.import_function_name;

                if constexpr (std::is_same_v<CurrentStdFuncInVariant, std::monostate>) { /* ... */
                } else if constexpr (std::is_same_v<CurrentStdFuncInVariant, HostFunc_ProcessBytesSignature>) {
                    local_linker
                        .func_wrap(
                            mod_name,
                            func_name,
                            [&](wasmtime::Caller caller, int32_t ptr, int32_t len) -> void {
                                if (this_comp != std::any_cast<WasmComponent*>(caller.context().get_data())) {
                                    throw wasmtime::Trap("Ctx mismatch in " + func_name);
                                }
                                uint32_t u_ptr = static_cast<uint32_t>(ptr);
                                uint32_t u_len = static_cast<uint32_t>(len);
                                std::vector<uint8_t> data_bytes;
                                try {
                                    data_bytes = this_comp->ReadBytesFromGuestMemory(u_ptr, u_len);
                                } catch (const std::exception& e) {
                                    throw wasmtime::Trap(
                                        ("Host func '" + func_name + "': err reading guest: " + std::string(e.what())).c_str());
                                }
                                try {
                                    func_impl_from_variant(caller, data_bytes);
                                } catch (const wasmtime::Trap& t) {
                                    throw;
                                } catch (const std::exception& e) {
                                    throw wasmtime::Trap(
                                        ("Host func '" + func_name + "': C++ threw: " + std::string(e.what())).c_str());
                                }
                            })
                        .unwrap();
                } else if constexpr (std::is_same_v<CurrentStdFuncInVariant, HostFunc_EngineLogSignature>) {
                    local_linker
                        .func_wrap(
                            mod_name,
                            func_name,
                            [&](wasmtime::Caller caller, int32_t p, int32_t l) -> void { func_impl_from_variant(caller, p, l); })
                        .unwrap();
                } else if constexpr (std::is_same_v<CurrentStdFuncInVariant, HostFunc_Void_Void>) {
                    local_linker
                        .func_wrap(mod_name, func_name, [&](wasmtime::Caller caller) -> void { func_impl_from_variant(caller); })
                        .unwrap();
                } else if constexpr (std::is_same_v<CurrentStdFuncInVariant, HostFunc_Void_Float>) {
                    local_linker
                        .func_wrap(mod_name,
                                   func_name,
                                   [&](wasmtime::Caller caller, float f) -> void { func_impl_from_variant(caller, f); })
                        .unwrap();
                } else if constexpr (std::is_same_v<CurrentStdFuncInVariant, HostFunc_Float_Void>) {
                    local_linker
                        .func_wrap(
                            mod_name, func_name, [&](wasmtime::Caller caller) -> float { return func_impl_from_variant(caller); })
                        .unwrap();
                } else { /* ... throw ... */
                }
            },
            binding.function_impl);
    }

    wasmtime::TrapResult<wasmtime::Instance> instance_trap_res = local_linker.instantiate(mStoreOpt->context(), module);

    if (!instance_trap_res) {
        return false;
    }
    mInstanceOpt.emplace(std::move(instance_trap_res.unwrap()));

    if (!ExtractExports()) {
        return false;
    }
    return true;
}

bool WasmComponent::ExtractExports() {
    if (!mInstanceOpt || !mStoreOpt) {
        LogMessage("WasmComponent [" + mId + "] ERROR (ExtractExports): Instance or Store is not valid.");
        return false;
    }

    auto store_ctx = mStoreOpt->context();

    std::optional<wasmtime::Extern> memory_extern_opt_variant = mInstanceOpt->get(store_ctx, "memory");
    if (memory_extern_opt_variant) {
        if (auto* mem_val = std::get_if<wasmtime::Memory>(&(*memory_extern_opt_variant))) {
            mMemoryOpt.emplace(*mem_val);
        } else {
            LogMessage("WasmComponent [" + mId + "] ERROR (ExtractExports): 'memory' export found but is not Memory type.");
            return false;
        }
    } else {
        LogMessage("WasmComponent [" + mId + "] ERROR (ExtractExports): 'memory' export not found. This is critical.");
        return false;
    }

    auto get_func_from_extern = [&](const std::string& name) -> std::optional<wasmtime::Func> {
        std::optional<wasmtime::Extern> extern_opt_variant = mInstanceOpt->get(store_ctx, name);
        if (extern_opt_variant) {
            if (auto* func_val = std::get_if<wasmtime::Func>(&(*extern_opt_variant))) {
                return *func_val;
            }
        }
        LogMessage("WasmComponent [" + mId + "] INFO: Export '" + name + "' not found or not a function.");
        return std::nullopt;
    };

    mAllocateFuncOpt = get_func_from_extern("allocate");
    mDeallocateFuncOpt = get_func_from_extern("deallocate");
    mInitializeFuncOpt = get_func_from_extern("initialize");
    mUpdateFuncOpt = get_func_from_extern("update");
    mOnMessageFuncOpt = get_func_from_extern("onMessage");
    mTerminateFuncOpt = get_func_from_extern("terminate");

    if (!mAllocateFuncOpt || !mDeallocateFuncOpt) {
        LogMessage("WasmComponent [" + mId +
                   "] WARN (ExtractExports): 'allocate' or 'deallocate' are missing. Host memory operations will fail.");
    }
    return true;
}

bool WasmComponent::TriggerWasmUpdate(float deltaTime) {
    if (!mIsInitialized || !mUpdateFuncOpt || !mStoreOpt) {
        return true;
    }
    try {
        mStoreOpt->context().set_fuel(100'000'000).unwrap();
        mStoreOpt->context().set_epoch_deadline(1);
        std::vector<wasmtime::Val> params;
        params.emplace_back(deltaTime);
        mUpdateFuncOpt->call(mStoreOpt->context(), params).unwrap();
    } catch (const wasmtime::Error& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM update call", e, mId);
        return false;
    } catch (const wasmtime::Trap& t) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM update call", t, mId);
        return false;
    } catch (const std::exception& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM update call", e, mId);
        return false;
    }
    return true;
}

bool WasmComponent::DeliverMessage(const std::string& fromId, const std::string& messageContent) {
    if (!mIsInitialized || !mOnMessageFuncOpt || !mStoreOpt) {
        return true;
    }

    uint32_t from_ptr = 0, msg_ptr = 0;
    uint32_t from_len = static_cast<uint32_t>(fromId.length());
    uint32_t msg_len = static_cast<uint32_t>(messageContent.length());
    bool success = false;

    try {
        if (from_len > 0) {
            from_ptr = AllocateInGuest(from_len);
            if (from_ptr == 0 && from_len > 0) throw std::runtime_error("Failed to allocate for from_id string (returned 0)");
            WriteStringToGuestMemory(from_ptr, fromId);
        }
        if (msg_len > 0) {
            msg_ptr = AllocateInGuest(msg_len);
            if (msg_ptr == 0 && msg_len > 0)
                throw std::runtime_error("Failed to allocate for message_content string (returned 0)");
            WriteStringToGuestMemory(msg_ptr, messageContent);
        }

        std::vector<wasmtime::Val> params;
        params.emplace_back(static_cast<int32_t>(from_ptr));
        params.emplace_back(static_cast<int32_t>(from_len));
        params.emplace_back(static_cast<int32_t>(msg_ptr));
        params.emplace_back(static_cast<int32_t>(msg_len));

        mStoreOpt->context().set_fuel(100'000'000).unwrap();
        mStoreOpt->context().set_epoch_deadline(1);
        mOnMessageFuncOpt->call(mStoreOpt->context(), params).unwrap();
        success = true;
    } catch (const wasmtime::Error& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM onMessage processing", e, mId);
        success = false;
    } catch (const wasmtime::Trap& t) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM onMessage processing", t, mId);
        success = false;
    } catch (const std::exception& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM onMessage processing", e, mId);
        success = false;
    }

    try {
        if (from_ptr != 0 && from_len > 0) DeallocateInGuest(from_ptr, from_len);
    } catch (...) {
    }
    try {
        if (msg_ptr != 0 && msg_len > 0) DeallocateInGuest(msg_ptr, msg_len);
    } catch (...) {
    }
    return success;
}

void WasmComponent::Terminate() {
    if (!mIsInitialized && !mInstanceOpt) {
        LogMessage("WasmComponent [" + mId + "]: Terminate called, but not initialized or already uninstantiated.");
        mIsInitialized = false;
        return;
    }
    if (mTerminateFuncOpt && mStoreOpt && mInstanceOpt) {
        try {
            mStoreOpt->context().set_fuel(100'000'000).unwrap();
            mStoreOpt->context().set_epoch_deadline(1);
            mTerminateFuncOpt->call(mStoreOpt->context(), {}).unwrap();
        } catch (const wasmtime::Error& e) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM terminate call", e, mId);
        } catch (const wasmtime::Trap& t) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM terminate call", t, mId);
        } catch (const std::exception& e) {
            if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM terminate call", e, mId);
        }
    } else {
        LogMessage("WasmComponent [" + mId + "]: No WASM 'terminate' function or instance/store not available.");
    }

    mIsInitialized = false;
    mMemoryOpt.reset();
    mInstanceOpt.reset();
    if (mStoreOpt) {
        mStoreOpt.reset();
    }
    mAllocateFuncOpt.reset();
    mDeallocateFuncOpt.reset();
    mInitializeFuncOpt.reset();
    mUpdateFuncOpt.reset();
    mOnMessageFuncOpt.reset();
    mTerminateFuncOpt.reset();
}

uint32_t WasmComponent::AllocateInGuest(uint32_t size) {
    if (!mIsInitialized || !mAllocateFuncOpt || !mStoreOpt) {
        throw std::runtime_error("AllocateInGuest: Component not ready or allocate function missing for " + mId);
    }
    if (size == 0) return 0;

    std::vector<wasmtime::Val> params;
    params.emplace_back(static_cast<int32_t>(size));

    auto results_vec = mAllocateFuncOpt->call(mStoreOpt->context(), params).unwrap();

    if (results_vec.empty() || results_vec[0].kind() != wasmtime::ValKind::I32) {
        throw std::runtime_error("AllocateInGuest: Wasm allocate function returned invalid type or no result for " + mId);
    }
    uint32_t ptr = static_cast<uint32_t>(results_vec[0].i32());
    if (ptr == 0 && size > 0) {
        LogMessage("WasmComponent [" + mId + "] WARN (AllocateInGuest): Guest 'allocate' for size " + std::to_string(size) +
                   " returned 0.");
    }
    return ptr;
}

void WasmComponent::DeallocateInGuest(uint32_t ptr, uint32_t size) {
    if (!mIsInitialized || !mDeallocateFuncOpt || !mStoreOpt) {
        LogMessage("WasmComponent [" + mId + "] WARN (DeallocateInGuest): Not ready to deallocate. Ptr: " + std::to_string(ptr));
        return;
    }
    if (ptr == 0) return;

    std::vector<wasmtime::Val> params;
    params.emplace_back(static_cast<int32_t>(ptr));
    params.emplace_back(static_cast<int32_t>(size));

    try {
        mDeallocateFuncOpt->call(mStoreOpt->context(), params).unwrap();
    } catch (const wasmtime::Error& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM DeallocateInGuest call", e, mId);
    } catch (const wasmtime::Trap& t) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM DeallocateInGuest call", t, mId);
    } catch (const std::exception& e) {
        if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM DeallocateInGuest call", e, mId);
    }
}

void WasmComponent::WriteBytesToGuestMemory(uint32_t guestPtr, const std::vector<uint8_t>& bytes) {
    if (!mIsInitialized || !mMemoryOpt || !mStoreOpt) throw std::runtime_error("WriteBytes: Not ready for " + mId);
    if (bytes.empty()) return;
    wasmtime::Span<uint8_t> mem_span = mMemoryOpt->data(mStoreOpt->context());
    if (guestPtr >= mem_span.size() || mem_span.size() - guestPtr < bytes.size())
        throw std::out_of_range("WriteBytes: Out of bounds for " + mId);
    std::memcpy(mem_span.data() + guestPtr, bytes.data(), bytes.size());
}

void WasmComponent::WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str) {
    WriteBytesToGuestMemory(guestPtr, std::vector<uint8_t>(str.begin(), str.end()));
}

std::vector<uint8_t> WasmComponent::ReadBytesFromGuestMemory(uint32_t guestPtr, uint32_t len) {
    if (len == 0) return {};
    if (!mIsInitialized || !mMemoryOpt || !mStoreOpt) throw std::runtime_error("ReadBytes: Not ready for " + mId);
    wasmtime::Span<uint8_t> mem_span = mMemoryOpt->data(mStoreOpt->context());
    if (guestPtr >= mem_span.size() || mem_span.size() - guestPtr < len)
        throw std::out_of_range("ReadBytes: Out of bounds for " + mId);
    const uint8_t* start_ptr = mem_span.data() + guestPtr;
    return std::vector<uint8_t>(start_ptr, start_ptr + len);
}

std::string WasmComponent::ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len) {
    std::vector<uint8_t> bytes = ReadBytesFromGuestMemory(guestPtr, len);
    return std::string(bytes.begin(), bytes.end());
}

}  // namespace EngineG::Wasm