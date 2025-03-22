#pragma once

#include <atomic>
#include <mutex>

namespace PhysicsEngine
{

struct Engine{
    std::atomic<bool> running{false};
    double x = 0.0, y = 0.0, z = 0.0;
    int tick = 0;

    std::mutex positionMutex;
};


}

