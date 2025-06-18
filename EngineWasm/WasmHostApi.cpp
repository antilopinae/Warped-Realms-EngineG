#include "WasmHostApi.hpp"

#include <iostream>

namespace EngineG::Wasm {

void LogMessage(const std::string& message) {
    std::cout << message << std::endl;
}

}  // namespace EngineG::Wasm