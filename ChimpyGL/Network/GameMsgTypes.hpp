#pragma once

#include <cstdint>

namespace EngineG::Network{

enum class GameMsgTypes: uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    GameWorldState,
    GamePlayerDisconnect,

    ClientPing,
    ClientInputUpdate,
    ClientRequestWorld,
};

}