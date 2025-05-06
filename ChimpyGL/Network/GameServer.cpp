#include "GameServer.hpp"
#include "NetMessage.hpp"
#include <iostream>
#include <thread>
#include <stdexcept>

namespace EngineG::Network {

GameServer::GameServer(uint16_t port)
    : ServerInterface<GameMsgTypes>(port),
      mLastUpdateTime(std::chrono::steady_clock::now()),
      mLastBroadcastTime(std::chrono::steady_clock::now())
{
    std::cout << "[GameServer] Initialized." << std::endl;
}

bool GameServer::OnClientConnect(std::shared_ptr<Connection<GameMsgTypes>> client) {
    uint32_t tentativeClientID = nIDCounter;
    std::cout << "[SERVER] Client attempting connection. Tentative ID: " << tentativeClientID << ". Approving..." << std::endl;

    client->Send(CreateMsgServerAccept(tentativeClientID));

    {
        std::scoped_lock lock(mPlayerMapMutex);
        auto& player = mPlayerMap[tentativeClientID];

        std::cout << "[SERVER] Creating Ship placeholder for client " << tentativeClientID << "..." << std::endl;
        // player.ship = std::make_unique<EngineG::Ship>(nullptr); // Pass nullptr for game context
        // if (player.ship) {
        //    player.ship->SetOwnerID(tentativeClientID);
        //    player.ship->SetPosition(EngineG::Vector2(512.0f + (tentativeClientID % 5) * 50.0f, 384.0f)); // Example spawn
        //    std::cout << "[SERVER] Player " << tentativeClientID << " Ship created." << std::endl;
        // } else {
        //     std::cerr << "[SERVER] CRITICAL: Failed to create Ship for client " << tentativeClientID << "! Player unusable." << std::endl;
        // }
         std::cout << "[SERVER] Player " << tentativeClientID << " entry created in map." << std::endl;
    }

    return true;
}

void GameServer::OnClientDisconnect(std::shared_ptr<Connection<GameMsgTypes>> client) {
    if (!client) return;

    uint32_t clientID = client->GetID();
    if (clientID == 0) {
        std::cerr << "[SERVER] Disconnect event for client with unassigned ID." << std::endl;
        return;
    }

    std::cout << "[SERVER] Client [" << clientID << "] disconnected. Removing player data..." << std::endl;

    {
        std::scoped_lock lock(mPlayerMapMutex);
        size_t erasedCount = mPlayerMap.erase(clientID);
        if(erasedCount > 0) {
             std::cout << "[SERVER] Player " << clientID << " removed from map." << std::endl;
        } else {
             std::cout << "[SERVER] Warning: Disconnect for client ID " << clientID << " not found in player map." << std::endl;
        }
    }

    Message<GameMsgTypes> msg = CreateMsgGamePlayerDisconnect(clientID);
    SendMessageAllClients(msg, client);
}

void GameServer::OnMessage(std::shared_ptr<Connection<GameMsgTypes>> client, const Message<GameMsgTypes>& message) {
    if (!client) return;

    uint32_t clientID = client->GetID();
    if (clientID == 0) {
         std::cerr << "[SERVER] Message received from client with unassigned ID. Ignoring." << std::endl;
         return;
    }

    switch (message.header.id) {
        case GameMsgTypes::ClientPing: {
            client->Send(message);
        }
        break;

        case GameMsgTypes::ClientInputUpdate: {
            PlayerInput input;
            auto msgCopy = message;
            try {
                msgCopy >> input;

                std::scoped_lock lock(mPlayerMapMutex);
                if (mPlayerMap.count(clientID)) {
                    mPlayerMap[clientID].lastInput = input;

                } else {
                    std::cerr << "[SERVER] Warning: Input update from unknown client ID " << clientID << "." << std::endl;
                }
            } catch (const std::runtime_error& e) {
                std::cerr << "[" << clientID << "] Error deserializing Client_InputUpdate: " << e.what() << std::endl;
            }
        }
        break;

        default:
            std::cout << "[SERVER] Unhandled message type from client " << clientID
                      << ": ID " << static_cast<uint32_t>(message.header.id) << std::endl;
            break;
    }
}

// --- Server Update and Broadcast ---

void GameServer::Update(size_t nMaxMessages, bool bWait) {
    // 1. Process incoming network messages (calls OnMessage)
    ServerInterface<GameMsgTypes>::Update(nMaxMessages, bWait);

    // 2. Simulate the game world
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - mLastUpdateTime).count();
    mLastUpdateTime = now;

    // Clamp delta time
    const float MAX_DELTA_TIME = 0.1f;
    deltaTime = std::min(deltaTime, MAX_DELTA_TIME);
     if (deltaTime <= 0.0f) return; // Skip update if no time passed


    // --- Game Simulation Step ---
    {
        std::scoped_lock lock(mPlayerMapMutex);
        for (auto& [id, player] : mPlayerMap) {
            // --- THIS REQUIRES REFACTORING ---
            // if (player.ship) {
            //     // 1. Apply player.lastInput to the ship's components/logic
            //     //    player.ship->ApplyInput(player.lastInput);
            //
            //     // 2. Update the ship's simulation state
            //     //    player.ship->Update(deltaTime);
            // }
            // --- END REFACTORING NOTE ---
        }
        // Update other server entities (projectiles, asteroids, etc.)
        // updateProjectiles(deltaTime);
        // checkCollisions();
    } // Mutex unlocked

    if (now - mLastBroadcastTime >= mBroadcastInterval) {
        // BroadcastWorldState();
        mLastBroadcastTime = now;
    }
}

// void GameServer::BroadcastWorldState() {
//     // Message<GameMsgTypes> msg = Message<GameMsgTypes>::CreateMsg_GameWorldState();
//
//     std::scoped_lock lock(mPlayerMapMutex);
//
//     // Add player count
//     uint32_t playerCount = static_cast<uint32_t>(mPlayerMap.size());
//     // msg << playerCount;
//
//     // Add state for each player
//     for (const auto& [id, player] : mPlayerMap) {
//         PlayerState state; // Create state struct to send
//         state.playerID = id;
//
//         // --- THIS REQUIRES REFACTORING ---
//         // Retrieve actual state from player.ship
//         // if (player.ship) {
//         //     const auto& pos = player.ship->GetPosition();
//         //     state.posX = pos.x;
//         //     state.posY = pos.y;
//         //     state.rotation = player.ship->GetRotation();
//         //
//         //     // Get velocity (example from a component)
//         //     // const auto& vel = player.ship->GetComponent<MoveComponent>()->GetVelocity();
//         //     // state.velX = vel.x;
//         //     // state.velY = vel.y;
//         // } else {
//             // Ship doesn't exist or failed creation, send default/placeholder state
//             state.posX = 0.0f;
//             state.posY = 0.0f;
//             state.rotation = 0.0f;
//             state.velX = 0.0f;
//             state.velY = 0.0f;
//         // }
//         // --- END REFACTORING NOTE ---
//
//         // Serialize the *entire* state struct
//         // msg << state;
//     }
//
//     // Send the completed message to all clients
//     // SendMessageAllClients(msg);
// }


} // namespace EngineG::Network