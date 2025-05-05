#pragma once

#include "NetClient.hpp"
#include "NetMessage.hpp"
#include "GameMsgTypes.hpp"

namespace EngineG::Network {

class GameClient: public ClientInterface<GameMsgTypes>
{
public:
  GameClient() = default;
public:
  void PingServer();
  void SendInputUpdate(const PlayerInput& currentInput);
  void RequestWorldState();
};

}