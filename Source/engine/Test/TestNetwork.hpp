#pragma once

#pragma once

#include "../Network.hpp"
#include <cassert>
#include <stdio.h>

namespace EngineG::Test
{
    class TestNetwork {
    public:
        TestNetwork(){
            try{
                StartNetwork();
            } catch(...){
                assert(false && "TestGame exception thrown");
            }
            printf("Network passed!\n");
        }

    private:
        void StartNetwork()
        {
            StartSimpleUDPServer();
        }
    };

}
