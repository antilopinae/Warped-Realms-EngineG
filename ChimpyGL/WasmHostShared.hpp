#pragma once

#include <functional>
#include <string>
#include <variant>
#include <vector>
#include <cstdint>
#include <wasmtime.hh>

// Общая для логгирования (WASM -> ptr, len)
using HostFunc_EngineLogSignature = std::function<void(wasmtime::Caller&, uint32_t ptr, uint32_t len)>;

// Для кастомной функции, где C++ ожидает std::vector<uint8_t>, а WASM передает (ptr, len)
using HostFunc_ProcessBytesSignature = std::function<void(wasmtime::Caller&, const std::vector<uint8_t>& bytes)>;

// Пример: функция без параметров, ничего не возвращает
using HostFunc_Void_Void = std::function<void(wasmtime::Caller&)>;

// Пример: функция, принимающая float, ничего не возвращает
using HostFunc_Void_Float = std::function<void(wasmtime::Caller&, float)>;

// Пример: функция без параметров, возвращающая float
using HostFunc_Float_Void = std::function<float(wasmtime::Caller&)>;

using HostFuncVariant = std::variant<
std::monostate,
HostFunc_EngineLogSignature,
HostFunc_ProcessBytesSignature,
HostFunc_Void_Void,
HostFunc_Void_Float,
HostFunc_Float_Void
>;

// Структура для описания привязки хост-функции
struct HostFunctionBinding {
    std::string import_module_name;   // Например, "env"
    std::string import_function_name; // Имя, как оно в WASM
    HostFuncVariant function_impl;    // C++ реализация
};
