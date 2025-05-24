#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

#include "SDL.hpp"
#include "Network.hpp"
// #include "WasmBridge.hpp"

#ifdef DEBUG
#define ASSETS_DIR "../../Assets/"

#else
#define ASSETS_DIR "Assets/"

#endif

constexpr char* WASM_FILE_PATH = ASSETS_DIR"simple_args.wasm";
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr char* FONT_PATH = ASSETS_DIR"Arial.ttf";
constexpr int FONT_SIZE = 24;

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
    void ProcessNetworkMessages();
    void SendInputToServer(const Uint8* keyState);

    Network::GameClient mClient;
    Network::GameServer mServer; // non-active

    uint32_t mPlayerID = 0;
    std::unordered_map<uint32_t, class Ship*> mOtherShips;
    bool mIsConnected = false;

    // Map of textures loaded
    std::unordered_map<std::string, SDL_Texture*> mTextures;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
    std::vector<class SpriteComponent*> mSprites;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    // Game-specific
    class Character* mCharcter;
    class Background* mBackground;
    class Ship* mShip;
    
    std::vector<class Asteroid*> mAsteroids;
};


}
