#include "GameClient.hpp"

namespace EngineG::Network {

void GameClient::PingServer() {
  Message<GameMsgTypes> msg = CreateMsgClientPing();
  Send(msg);
}

void GameClient::SendInputUpdate(const PlayerInput& currentInput) {
  Message<GameMsgTypes> msg = CreateMsgClientInputUpdate(currentInput);
  Send(msg);
}

}