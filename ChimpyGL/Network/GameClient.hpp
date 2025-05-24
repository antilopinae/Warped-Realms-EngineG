#pragma once

#include "GameMsgTypes.hpp"
#include "NetClient.hpp"
#include "NetMessage.hpp"

namespace EngineG::Network
{

class GameClient : public ClientInterface<GameMsgTypes>
{
  public:
  GameClient() = default;

  public:
  void PingServer();
  void SendInputUpdate(const PlayerInput& currentInput);
  void RequestWorldState();
};

}    // namespace EngineG::Network