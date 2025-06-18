#pragma once

#include "Network/Network.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include "Wasm/WasmNetworkManager.hpp"
#include "Wasm/WasmIntegrationService.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef DEBUG
#define ASSETS_DIR "../../Assets/"

#else
#define ASSETS_DIR "../Assets/"

#endif

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr const char* FONT_PATH = ASSETS_DIR "Arial.ttf";
constexpr int FONT_SIZE = 24;

namespace EngineG {

class Game {
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    Actor* FindActorById(const std::string& actorId);
    // Actor* CreateActorOfType(const std::string& actorType, const std::string& desiredId);

    Wasm::WasmNetworkManager* GetNetworkedWasmManager() const { return mWasmNetworkManager.get(); }
    bool ApplyWasmToActorComponent(const std::string& targetActorId,
                                   const std::string& scriptSlotNameOnComponent,
                                   const std::string& localWasmPath);

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    SDL_Texture* GetTexture(const std::string& fileName);
    std::vector<std::string> GetCSV(const std::string& fileName);
    WasmIntegrationService& GetWasmService() { return mWasmIntegrationService; }

    // Game-specific (add/remove asteroid)
    void AddAsteroid(class Asteroid* ast);
    void RemoveAsteroid(class Asteroid* ast);
    std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

    // Game-specific (tower defense)
    class Grid* GetGrid() { return mGrid; }
    std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
    class Enemy* GetNearestEnemy(const Vector2& pos);

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // --- Networking ---
    void ProcessNetworkMessages();
    void SendInputToServer(const Uint8* keyState);

    Network::GameClient mClient;
    Network::GameServer mServer;  // non-active

    uint32_t mPlayerID = 0;
    std::unordered_map<uint32_t, class Ship*> mOtherShips;
    bool mIsConnected = false;

    // Map of textures loaded
    std::unordered_map<std::string, SDL_Texture*> mTextures;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
    std::vector<class SpriteComponent*> mSprites;

    // Для поиска акторов по ID
    // Либо mActors будет std::map<std::string, std::unique_ptr<Actor>>

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    std::unique_ptr<Wasm::WasmNetworkManager> mWasmNetworkManager;
    WasmIntegrationService mWasmIntegrationService;

    // Game-specific
    class Character* mCharcter;
    class Background* mBackground;
    class Ship* mShip;

    std::vector<class Asteroid*> mAsteroids;

    // Tower Defense
    std::vector<class Enemy*> mEnemies;
    class Grid* mGrid;
    float mNextEnemy;
};

}  // namespace EngineG
