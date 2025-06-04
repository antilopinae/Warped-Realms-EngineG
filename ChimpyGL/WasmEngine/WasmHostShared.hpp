#pragma once

#include "WasmHostApi.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <wasmtime.hh>

namespace EngineG::Wasm {
using HostFunc_EngineLogSignature = std::function<void(wasmtime::Caller&, int32_t ptr, int32_t len)>;  // ИЗМЕНЕНО
using HostFunc_ProcessBytesSignature = std::function<void(wasmtime::Caller&, const std::vector<uint8_t>& bytes)>;

using HostFunc_Void_Void = std::function<void(wasmtime::Caller&)>;
using HostFunc_Void_Float = std::function<void(wasmtime::Caller&, float)>;
using HostFunc_Float_Void = std::function<float(wasmtime::Caller&)>;

using HostFuncVariant = std::variant<std::monostate,
                                     HostFunc_EngineLogSignature,
                                     HostFunc_ProcessBytesSignature,
                                     HostFunc_Void_Void,
                                     HostFunc_Void_Float,
                                     HostFunc_Float_Void>;

struct HostFunctionBinding {
    std::string import_module_name;    // "env"
    std::string import_function_name;  // Имя, как оно в WASM
    HostFuncVariant function_impl;     // C++ реализация

    HostFunctionBinding(std::string&& module, std::string&& func, HostFuncVariant&& impl)
    : import_module_name(std::forward<std::string>(module)),
    import_function_name(std::forward<std::string>(func)),
    function_impl(std::forward<HostFuncVariant>(impl)) {}
};
}  // namespace EngineG::Wasm
