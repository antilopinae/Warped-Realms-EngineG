#include "WasmComponent.hpp"

#include <cstring>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "GameEngine.hpp"
#include "HostApi.hpp"

namespace WasmHost {

WasmComponent::WasmComponent(std::string componentId, std::string wasmPath, std::string entityName, std::string slotName,
                             GameEngine* ownerEngine, wasmtime::Engine& engine_ref)
    : mId(std::move(componentId)), mPath(std::move(wasmPath)), mOwnerEngine(ownerEngine), mEngineRef(engine_ref) {
  (void)entityName;
  (void)slotName;
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
      mStoreOpt.reset();
      return false;
    }
    mIsInitialized = true;

    if (mInitializeFuncOpt) {
      mInitializeFuncOpt->call(mStoreOpt->context(), {}).unwrap();
    } else {
      LogMessage("WasmComponent [" + mId + "]: No WASM 'initialize' function found/exported.");
    }
  } catch (const wasmtime::Error& e) {
    if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM Initialize (Error)", e, mId);
    mStoreOpt.reset();
    mInstanceOpt.reset();
    mMemoryOpt.reset();
    return false;
  } catch (const wasmtime::Trap& t) {
    if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM Initialize (Trap)", t, mId);
    mStoreOpt.reset();
    mInstanceOpt.reset();
    mMemoryOpt.reset();
    return false;
  } catch (const std::exception& e) {
    if (mOwnerEngine) mOwnerEngine->LogWasmException("WASM Initialize (StdExc)", e, mId);
    mStoreOpt.reset();
    mInstanceOpt.reset();
    mMemoryOpt.reset();
    return false;
  }

  mIsInitialized = true;
  return true;
}

bool WasmComponent::LoadAndInstantiateModule() {
  if (!mOwnerEngine->ReadFileToVector(mPath, mWasmBinaryData)) {
    LogMessage("WasmComponent [" + mId + "] ERROR: Failed to read WASM file: " + mPath);
    return false;
  }
  if (mWasmBinaryData.empty()) {
    LogMessage("WasmComponent [" + mId + "] ERROR: WASM file is empty: " + mPath);
    return false;
  }

  wasmtime::Result<wasmtime::Module> module_res = wasmtime::Module::compile(mEngineRef, mWasmBinaryData);
  if (!module_res) {
    LogMessage("WasmComponent [" + mId + "] ERROR: Failed to compile WASM module: " + module_res.err().message());
    if (mOwnerEngine) mOwnerEngine->LogWasmException("LoadAndInstantiateModule - Module::compile", module_res.err(), mId);
    return false;
  }
  wasmtime::Module module = std::move(module_res.unwrap());

  mInstanceOpt.emplace(mOwnerEngine->GetLinker().instantiate(mStoreOpt->context(), module).unwrap());

  if (!ExtractExports()) {
    mInstanceOpt.reset();
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

bool WasmComponent::TriggerWasmUpdate() {
  if (!mIsInitialized || !mUpdateFuncOpt || !mStoreOpt) {
    return true;
  }
  try {
    mStoreOpt->context().set_fuel(100'000'000).unwrap();
    mStoreOpt->context().set_epoch_deadline(1);
    mUpdateFuncOpt->call(mStoreOpt->context(), {}).unwrap();
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
      if (msg_ptr == 0 && msg_len > 0) throw std::runtime_error("Failed to allocate for message_content string (returned 0)");
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

void WasmComponent::WriteStringToGuestMemory(uint32_t guestPtr, const std::string& str) {
  if (!mIsInitialized || !mMemoryOpt || !mStoreOpt) {
    throw std::runtime_error("WriteStringToGuestMemory: Component not ready for " + mId);
  }
  if (str.empty()) return;

  size_t str_len = str.length();
  auto store_ctx = mStoreOpt->context();
  wasmtime::Span<uint8_t> mem_span = mMemoryOpt->data(store_ctx);
  uint8_t* mem_data_ptr = mem_span.data();
  size_t current_mem_byte_size = mem_span.size();

  if (guestPtr >= current_mem_byte_size || current_mem_byte_size - guestPtr < str_len) {
    throw std::out_of_range("WriteStringToGuestMemory: Out of bounds for " + mId);
  }
  std::memcpy(mem_data_ptr + guestPtr, str.data(), str_len);
}

std::string WasmComponent::ReadStringFromGuestMemory(uint32_t guestPtr, uint32_t len) {
  if (len == 0) return "";
  if (!mIsInitialized || !mMemoryOpt || !mStoreOpt) {
    return "[ReadError:WasmComponentNotReady:" + mId + "]";
  }

  auto store_ctx = mStoreOpt->context();
  wasmtime::Span<uint8_t> mem_span = mMemoryOpt->data(store_ctx);
  uint8_t* mem_data_ptr = mem_span.data();
  size_t current_mem_byte_size = mem_span.size();

  if (guestPtr >= current_mem_byte_size || current_mem_byte_size - guestPtr < len) {
    return "[ReadError:OutOfBounds:" + mId + "]";
  }
  try {
    return std::string(reinterpret_cast<const char*>(mem_data_ptr + guestPtr), len);
  } catch (const std::exception& e) {
    return "[ReadError:ExceptionAtConstruct:" + mId + ":" + e.what() + "]";
  }
}

}  // namespace WasmHost