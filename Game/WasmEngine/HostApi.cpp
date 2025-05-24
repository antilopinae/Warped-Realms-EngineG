#include "HostApi.hpp"
#include <iostream>

namespace WasmHost {

void LogMessage(const std::string& message) {
  std::cout << message << std::endl;
}

} // namespace WasmHost