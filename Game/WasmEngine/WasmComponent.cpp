#include "WasmComponent.hpp"
#include "GameEngine.hpp"
#include "HostApi.hpp"
#include <vector>
#include <stdexcept>
#include <cstring> // Для memcpy

namespace WasmHost {

static wasm_trap_t* HostLogWasmImpl(void* env, const wasm_val_vec_t* args, wasm_val_vec_t* /*results*/) {
    WasmComponent* component = static_cast<WasmComponent*>(env);
    if (!component || !component->GetOwnerEngine()) {
         return wasmtime_trap_new("HostLogWasmImpl: env or owner_engine is null", strlen("..."));
    }
    if (args->size < 2 || args->data[0].kind != WASM_I32 || args->data[1].kind != WASM_I32) {
        return wasmtime_trap_new("HostLogWasmImpl: invalid arguments", strlen("..."));
    }
    uint32_t ptr = static_cast<uint32_t>(args->data[0].of.i32);
    uint32_t len = static_cast<uint32_t>(args->data[1].of.i32);

    // auto msg_content = std::string(reinterpret_cast<const char*>(component->GetGuestMemoryDataPtr()+ptr), len);
    std::string msg = component->ReadStringFromGuestMemory(ptr, len);

    LogMessage("WASM " + msg);

    return nullptr;
}

static wasm_trap_t* HostStubNotifyMemoryGrowthImpl(void* /*env*/, const wasm_val_vec_t* /*args*/, wasm_val_vec_t* /*results*/) {
  return nullptr;
}

WasmComponent::WasmComponent(std::string componentId, std::string wasmPath, std::string entityName, std::string slotName,
                             GameEngine* ownerEngine, wasm_engine_t* engine)
    : mId(std::move(componentId)),
      mPath(std::move(wasmPath)),
      mEntityName(std::move(entityName)),
      mSlotName(std::move(slotName)),
      mOwnerEngine(ownerEngine) {
    if (!engine) {
        throw std::runtime_error("WasmComponent: Engine cannot be null.");
    }
    mStore = wasm_store_new(engine);
    if (!mStore) {
        throw std::runtime_error("WasmComponent: Failed to create wasm_store_t for " + mId);
    }
}

WasmComponent::~WasmComponent() {
    if (mAllocateFunc) wasm_func_delete(mAllocateFunc);
    if (mDeallocateFunc) wasm_func_delete(mDeallocateFunc);
    if (mInitializeFuncFromWasm) wasm_func_delete(mInitializeFuncFromWasm);
    if (mUpdateFuncFromWasm) wasm_func_delete(mUpdateFuncFromWasm);
    if (mOnMessageFuncFromWasm) wasm_func_delete(mOnMessageFuncFromWasm);
    if (mTerminateFuncFromWasm) wasm_func_delete(mTerminateFuncFromWasm);

    if (mInstance) wasm_instance_delete(mInstance);
    if (mModule) wasm_module_delete(mModule);
    if (mStore) wasm_store_delete(mStore);
}

bool WasmComponent::Initialize() {
    if (mIsInitialized) return true;

    if (!LoadModuleAndInstantiate(nullptr)) {
        LogMessage("WasmComponent " + mId + " ERROR: Failed to load and instantiate module.");
        return false;
    }

    if (mInitializeFuncFromWasm) {
        if (!CallVoidExport(mInitializeFuncFromWasm, "WASM initialize")) {
            LogMessage("WasmComponent " + mId + " ERROR: Execution of WASM initialize function failed.");
            if(mInstance) wasm_instance_delete(mInstance); mInstance = nullptr;
            if(mModule) wasm_module_delete(mModule); mModule = nullptr;
            return false;
        }
    }
    mIsInitialized = true;
    LogMessage("WasmComponent " + mId + " initialized successfully (host and WASM).");
    return true;
}

bool WasmComponent::LoadModuleAndInstantiate(wasm_engine_t* /*engine_ptr_ignored*/) {
    if (!mOwnerEngine->ReadFileToVector(mPath, mWasmBinaryData)) {
        return false;
    }

    wasm_byte_vec_t wasm_byte_vec;
    wasm_byte_vec_new(&wasm_byte_vec, mWasmBinaryData.size(), mWasmBinaryData.data());

    mModule = wasm_module_new(mStore, &wasm_byte_vec);
    wasm_byte_vec_delete(&wasm_byte_vec);

    if (!mModule) {
        LogMessage("WasmComponent " + mId + " ERROR: Failed to compile WASM module.");
        return false;
    }

    if (!LinkImports()) {
        wasm_module_delete(mModule); mModule = nullptr;
        return false;
    }

    wasm_extern_vec_t exports_vec;
    wasm_instance_exports(mInstance, &exports_vec);

    wasm_exporttype_vec_t module_export_types;
    wasm_module_exports(mModule, &module_export_types);

    bool memory_found = false;
    for (size_t i = 0; i < exports_vec.size && i < module_export_types.size; ++i) {
        wasm_extern_t* ext = exports_vec.data[i];
        const wasm_name_t* export_name_vec = wasm_exporttype_name(module_export_types.data[i]);
        std::string export_name(export_name_vec->data, export_name_vec->size);

        if (wasm_extern_kind(ext) == WASM_EXTERN_FUNC) {
            wasm_func_t* func = wasm_extern_as_func(ext);
            if (export_name == "allocate") mAllocateFunc = wasm_func_copy(func);
            else if (export_name == "deallocate") mDeallocateFunc = wasm_func_copy(func);
            else if (export_name == "initialize") mInitializeFuncFromWasm = wasm_func_copy(func);
            else if (export_name == "update") mUpdateFuncFromWasm = wasm_func_copy(func);
            else if (export_name == "onMessage") mOnMessageFuncFromWasm = wasm_func_copy(func);
            else if (export_name == "terminate") mTerminateFuncFromWasm = wasm_func_copy(func);
        } else if (wasm_extern_kind(ext) == WASM_EXTERN_MEMORY && export_name == "memory") {
            mMemory = wasm_extern_as_memory(ext);
            memory_found = true;
        }
    }
    wasm_exporttype_vec_delete(&module_export_types);
    wasm_extern_vec_delete(&exports_vec);

    if (!memory_found) {
        LogMessage("WasmComponent " + mId + " ERROR: Exported 'memory' not found.");
        if(mInstance) wasm_instance_delete(mInstance); mInstance = nullptr;
        if(mModule) wasm_module_delete(mModule); mModule = nullptr;
        return false;
    }
    return true;
}

bool WasmComponent::LinkImports() {
    if (!mStore || !mModule) {
        LogMessage("WasmComponent " + mId + " ERROR: Store or Module is null.");
        return false;
    }

    wasm_importtype_vec_t needed_imports_types;
    wasm_module_imports(mModule, &needed_imports_types);

    std::vector<wasm_extern_t*> resolved_extern_imports(needed_imports_types.size);
    bool all_resolved = true;

    wasm_func_t* host_log_func = nullptr;
    wasm_func_t* host_notify_func = nullptr;

    bool needs_log = false;
    bool needs_notify = false;

    for (size_t i = 0; i < needed_imports_types.size; ++i) {
        const wasm_name_t* module_name = wasm_importtype_module(needed_imports_types.data[i]);
        const wasm_name_t* field_name = wasm_importtype_name(needed_imports_types.data[i]);

        if (!module_name || !field_name || !module_name->data || !field_name->data) {
            LogMessage("WasmComponent " + mId + " ERROR: Null import name.");
            all_resolved = false;
            break;
        }

        std::string mod(module_name->data, module_name->size);
        std::string field(field_name->data, field_name->size);

        if (mod == "env") {
            if (field == "engine_log_wasm") needs_log = true;
            else if (field == "emscripten_notify_memory_growth") needs_notify = true;
        }
    }

    if (needs_log) {
        wasm_valtype_t* params[] = { wasm_valtype_new_i32(), wasm_valtype_new_i32() };
        wasm_valtype_vec_t param_vec;
        wasm_valtype_vec_new(&param_vec, 2, params);
        wasm_valtype_vec_t result_vec;
        wasm_valtype_vec_new_empty(&result_vec);

        wasm_functype_t* func_type = wasm_functype_new(&param_vec, &result_vec);
        wasm_valtype_vec_delete(&param_vec);
        wasm_valtype_vec_delete(&result_vec);

        if (func_type) {
            host_log_func = wasm_func_new_with_env(mStore, func_type, HostLogWasmImpl, this, nullptr);
            wasm_functype_delete(func_type);

            if (!host_log_func) {
                LogMessage("WasmComponent " + mId + " ERROR: Failed to create engine_log_wasm func.");
                all_resolved = false;
            }
        } else {
            LogMessage("WasmComponent " + mId + " ERROR: Failed to create functype for engine_log_wasm.");
            all_resolved = false;
        }
    }

    if (needs_notify && all_resolved) {
        wasm_valtype_t* params[] = { wasm_valtype_new_i32() };
        wasm_valtype_vec_t param_vec;
        wasm_valtype_vec_new(&param_vec, 1, params);
        wasm_valtype_vec_t result_vec;
        wasm_valtype_vec_new_empty(&result_vec);

        wasm_functype_t* func_type = wasm_functype_new(&param_vec, &result_vec);
        wasm_valtype_vec_delete(&param_vec);
        wasm_valtype_vec_delete(&result_vec);

        if (func_type) {
            host_notify_func = wasm_func_new_with_env(mStore, func_type, HostStubNotifyMemoryGrowthImpl, nullptr, nullptr);
            wasm_functype_delete(func_type);
            if (!host_notify_func) {
                LogMessage("WasmComponent " + mId + " WARNING: emscripten_notify_memory_growth not created.");
            }
        } else {
            LogMessage("WasmComponent " + mId + " ERROR: Failed to create functype for emscripten_notify_memory_growth.");
        }
    }

    if (all_resolved) {
        for (size_t i = 0; i < needed_imports_types.size; ++i) {
            const wasm_name_t* module_name = wasm_importtype_module(needed_imports_types.data[i]);
            const wasm_name_t* field_name = wasm_importtype_name(needed_imports_types.data[i]);
            std::string mod(module_name->data, module_name->size);
            std::string field(field_name->data, field_name->size);

            resolved_extern_imports[i] = nullptr;

            if (mod == "env") {
                if (field == "engine_log_wasm" && host_log_func) {
                    resolved_extern_imports[i] = wasm_func_as_extern(host_log_func);
                } else if (field == "emscripten_notify_memory_growth" && host_notify_func) {
                    resolved_extern_imports[i] = wasm_func_as_extern(host_notify_func);
                } else {
                    LogMessage("WasmComponent " + mId + " ERROR: Unresolved import: env." + field);
                    all_resolved = false;
                    break;
                }
            } else {
                LogMessage("WasmComponent " + mId + " ERROR: Unknown import module: " + mod);
                all_resolved = false;
                break;
            }
        }
    }

    wasm_importtype_vec_delete(&needed_imports_types);

    if (!all_resolved) {
        if (host_log_func) wasm_func_delete(host_log_func);
        if (host_notify_func) wasm_func_delete(host_notify_func);
        LogMessage("WasmComponent " + mId + " ERROR: Failed to resolve imports.");
        return false;
    }

    wasm_extern_vec_t imports_vec = { resolved_extern_imports.size(), resolved_extern_imports.data() };
    wasm_trap_t* trap = nullptr;
    mInstance = wasm_instance_new(mStore, mModule, &imports_vec, &trap);

    if (host_log_func) wasm_func_delete(host_log_func);
    if (host_notify_func) wasm_func_delete(host_notify_func);

    if (trap) {
        mOwnerEngine->HandleWasmTrap("LinkImports (instantiation)", trap, mId);
        return false;
    }

    if (!mInstance) {
        LogMessage("WasmComponent " + mId + " ERROR: Instantiation failed (null instance, no trap).");
        return false;
    }

    return true;
}


bool WasmComponent::CallVoidExport(wasm_func_t* func, const char* funcNameForLog) {
    if (!func) return true;
    if (!mIsInitialized && func != mInitializeFuncFromWasm) {
         LogMessage("WasmComponent " + mId + " WARN: Attempt to call " + funcNameForLog + " on uninitialized component.");
    }

    wasm_val_vec_t args;
    wasm_val_vec_new_empty(&args);
    wasm_val_vec_t results;
    wasm_val_vec_new_empty(&results);
    wasm_trap_t* trap = wasm_func_call(func, &args, &results);
    wasm_val_vec_delete(&results);
    wasm_val_vec_delete(&args);

    if (trap) { mOwnerEngine->HandleWasmTrap(funcNameForLog, trap, mId); return false; }
    return true;
}

bool WasmComponent::Update() {
    if (!mIsInitialized) return true;

    if (!mUpdateFuncFromWasm || !mStore || !mInstance) {
      LogMessage("WasmComponent " + mId + " FATAL ERROR: params is NULL before CallVoidExport!");
      return false;
    }
    return CallVoidExport(mUpdateFuncFromWasm, "WASM update");
}

bool WasmComponent::DeliverMessage(const std::string& fromId, const std::string& message) {
    if (!mIsInitialized || !mOnMessageFuncFromWasm) return true;

    uint32_t from_ptr = 0;
    uint32_t from_len = 0;
    if (!fromId.empty()) {
        from_ptr = AllocateInGuest(fromId.length());
        if (from_ptr == 0 && fromId.length() > 0) { return false; }
        WriteStringToGuestMemory(from_ptr, fromId);
        from_len = fromId.length();
    }

    uint32_t msg_ptr = 0;
    uint32_t msg_len = 0;
    if(!message.empty()){
        msg_ptr = AllocateInGuest(message.length());
        if (msg_ptr == 0 && message.length() > 0) {
            if(from_ptr) DeallocateInGuest(from_ptr, from_len);
            return false;
        }
        WriteStringToGuestMemory(msg_ptr, message);
        msg_len = message.length();
    }

    wasm_val_t args_val[4] = {
        WASM_I32_VAL(static_cast<int32_t>(from_ptr)), WASM_I32_VAL(static_cast<int32_t>(from_len)),
        WASM_I32_VAL(static_cast<int32_t>(msg_ptr)), WASM_I32_VAL(static_cast<int32_t>(msg_len))
    };
    wasm_val_vec_t c_args = WASM_ARRAY_VEC(args_val);
    wasm_val_vec_t c_results; wasm_val_vec_new_empty(&c_results);

    wasm_trap_t* trap = wasm_func_call(mOnMessageFuncFromWasm, &c_args, &c_results);
    wasm_val_vec_delete(&c_results);

    if (from_ptr) DeallocateInGuest(from_ptr, from_len);
    if (msg_ptr) DeallocateInGuest(msg_ptr, msg_len);

    if (trap) { mOwnerEngine->HandleWasmTrap("WASM onMessage", trap, mId); return false; }
    return true;
}

bool WasmComponent::Terminate() {
    if (!mIsInitialized && !mInstance) return true;
    bool success = CallVoidExport(mTerminateFuncFromWasm, "WASM terminate");
    mIsInitialized = false;
    return success;
}

byte_t* WasmComponent::GetGuestMemoryDataPtr() {
    if (!mMemory) return nullptr;
    return wasm_memory_data(mMemory);
}

size_t WasmComponent::GetGuestMemoryDataSize() {
    if (!mMemory) return 0;
    return wasm_memory_data_size(mMemory);
}

void WasmComponent::WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str) {
    byte_t* memData = GetGuestMemoryDataPtr();
    if (!memData || (guestPtr + str.length() > GetGuestMemoryDataSize() && str.length() > 0) ) {
        LogMessage("WasmComponent " + mId + " ERROR: WriteString Out of bounds/No mem. Ptr: " + std::to_string(guestPtr) + ", StrLen: " + std::to_string(str.length()) + ", MemSize: " + std::to_string(GetGuestMemoryDataSize()));
        return;
    }
    if(!str.empty()) memcpy(memData + guestPtr, str.data(), str.length());
}

std::string WasmComponent::ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len) {
    byte_t* memData = GetGuestMemoryDataPtr();
    if (!memData || (guestPtr + len > GetGuestMemoryDataSize() && len > 0) ) {
        LogMessage("WasmComponent " + mId + " ERROR: ReadString Out of bounds/No mem. Ptr: " + std::to_string(guestPtr) + ", Len: " + std::to_string(len) + ", MemSize: " + std::to_string(GetGuestMemoryDataSize()));
        return "";
    }
    if (len == 0) return "";
    return std::string(reinterpret_cast<const char*>(memData + guestPtr), len);
}

uint32_t WasmComponent::AllocateInGuest(uint32_t size) {
    if (!mAllocateFunc) {
        LogMessage("WasmComponent " + mId + " ERROR: AllocateInGuest - mAllocateFunc is null.");
        return 0;
    }
    if (!mIsInitialized && size > 0) {
        LogMessage("WasmComponent " + mId + " WARN: AllocateInGuest called on uninitialized component.");
    }

    wasm_val_t args_val[1] = { WASM_I32_VAL(static_cast<int32_t>(size)) };
    wasm_val_t results_val[1];
    wasm_val_vec_t args = WASM_ARRAY_VEC(args_val);
    wasm_val_vec_t results = WASM_ARRAY_VEC(results_val);

    wasm_trap_t* trap = wasm_func_call(mAllocateFunc, &args, &results);
    if (trap) {
        mOwnerEngine->HandleWasmTrap("AllocateInGuest", trap, mId);
        return 0;
    }
    if (results_val[0].kind != WASM_I32) {
        LogMessage("WasmComponent " + mId + " ERROR: mAllocateFunc did not return I32.");
        return 0;
    }
    return static_cast<uint32_t>(results_val[0].of.i32);
}

void WasmComponent::DeallocateInGuest(uint32_t ptr, uint32_t size) {
    if (!mDeallocateFunc) {
        LogMessage("WasmComponent " + mId + " ERROR: DeallocateInGuest - mDeallocateFunc is null.");
        return;
    }
     if (!mIsInitialized && ptr > 0) {
        LogMessage("WasmComponent " + mId + " WARN: DeallocateInGuest called on uninitialized component.");
    }

    wasm_val_t args_val[2] = { WASM_I32_VAL(static_cast<int32_t>(ptr)), WASM_I32_VAL(static_cast<int32_t>(size)) };
    wasm_val_vec_t args = WASM_ARRAY_VEC(args_val);
    wasm_val_vec_t results_vec;
    wasm_val_vec_new_empty(&results_vec);

    wasm_trap_t* trap = wasm_func_call(mDeallocateFunc, &args, &results_vec);
    wasm_val_vec_delete(&results_vec);

    if (trap) {
        mOwnerEngine->HandleWasmTrap("DeallocateInGuest", trap, mId);
    }
}

} // namespace WasmHost