#pragma once

#include "NetServer.hpp"
#include "GameMsgTypes.hpp"
#include "PlayerStates.hpp"

#include "../Actor.hpp"

#include <unordered_map>
#include <chrono>
#include <memory>
#include <mutex>

namespace EngineG::Network {

struct Player {
    std::unique_ptr<EngineG::Ship> ship = nullptr;

    PlayerInput lastInput;
    PlayerState lastState;
};

class GameServer : public ServerInterface<GameMsgTypes> {
public:
    GameServer(uint16_t port);
    virtual ~GameServer() = default;

protected:
    bool OnClientConnect(std::shared_ptr<Connection<GameMsgTypes>> client) override;
    void OnClientDisconnect(std::shared_ptr<Connection<GameMsgTypes>> client) override;
    void OnMessage(std::shared_ptr<Connection<GameMsgTypes>> client, const Message<GameMsgTypes>& message) override;

public:
    void Update(size_t nMaxMessages = static_cast<size_t>(-1), bool bWait = false);

private:
    std::unordered_map<uint32_t /*Client ID*/, Player> mPlayerMap;
    std::mutex mPlayerMapMutex;

    std::chrono::steady_clock::time_point mLastUpdateTime;
    std::chrono::steady_clock::time_point mLastBroadcastTime;
    // Send world state roughly 30 times per second (adjust as needed)
    const std::chrono::duration<float> mBroadcastInterval = std::chrono::milliseconds(33);
};

inline void startGameServer(uint16_t port = 60000)
{
    GameServer server(port);
    if (server.Start()) {
        std::cout << "[SERVER] Game Server started successfully on port " << port << "." << std::endl;
        while (true)
        {
            server.Update(10, true);
        }
    } else {
        std::cerr << "[SERVER] FATAL: Failed to start server on port " << port << "." << std::endl;
    }
}

} // namespace EngineG::Network