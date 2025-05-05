#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

#include "SDL.hpp"
#include "Network.hpp"

namespace EngineG{

class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    SDL_Texture* GetTexture(const std::string& fileName);
    std::vector<std::string> GetCSV(const std::string& fileName);

    // Game-specific (add/remove asteroid)
    void AddAsteroid(class Asteroid* ast);
    void RemoveAsteroid(class Asteroid* ast);
    std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // --- Networking ---
    void ProcessNetworkMessages(); // New function to handle incoming messages
    void SendInputToServer();     // New function to send local input

    GameClient mClient;          // The network client instance
    uint32_t mPlayerID = 0;      // This client's unique ID assigned by the server
    // Map of remote player IDs to their Ship actors
    std::unordered_map<uint32_t, class Ship*> mOtherPlayerShips;
    bool mIsConnected = false;   // Track connection status

    // Map of textures loaded
    std::unordered_map<std::string, SDL_Texture*> mTextures;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    // Any pending actors
    std::vector<class Actor*> mPendingActors;

    // All the sprite components drawn
    std::vector<class SpriteComponent*> mSprites;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    // Game-specific
    // class Ship* mShip; // Player's ship
    class Character* mCharcter;
    class Background* mBackground;

    // Game-specific
    class Ship* mShip; // Player's ship
    std::vector<class Asteroid*> mAsteroids;
};


}
