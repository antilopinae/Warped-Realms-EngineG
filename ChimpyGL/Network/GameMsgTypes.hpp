#pragma once

#include <cstdint>

namespace EngineG::Network{

enum class GameMsgTypes: uint32_t
{
    ServerAccept,
    ServerPing,
    GamePlayerDisconnect,

    ClientPing,
    ClientInputUpdate
};

}