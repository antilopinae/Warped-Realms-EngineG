#pragma once

#pragma once

#include <stdio.h>

#include "../Network.hpp"
#include <cassert>

namespace EngineG::Test {
class TestNetwork {
public:
    TestNetwork() {
        try {
            StartNetwork();
        } catch (...) {
            assert(false && "TestGame exception thrown");
        }
        printf("Network passed!\n");
    }

private:
    void StartNetwork() { StartSimpleUDPServer(); }
};

}  // namespace EngineG::Test
