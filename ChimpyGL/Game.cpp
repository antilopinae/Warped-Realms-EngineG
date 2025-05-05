#include "Game.hpp"

#include "Actor.hpp"
#include "Math.hpp"

#include <fstream>
#include <iostream>

namespace EngineG{

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{

}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1024, 768, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    Random::Init();

    LoadData();

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
      switch (event.type)
      {
          case SDL_QUIT:
              mIsRunning = false;
              break;
      }
  }

  const Uint8* keyState = SDL_GetKeyboardState(NULL);
  if (keyState[SDL_SCANCODE_ESCAPE])
  {
      mIsRunning = false;
  }

#if 0
  // Process ship input
  // mShip->ProcessKeyboard(state);
  mCharcter->ProcessKeyboard(state);
#endif

    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->ProcessInput(keyState);
    }
    mUpdatingActors = false;
}

void Game::SendInputToServer() {
  if (!mIsConnected || !mPlayerShip) return;

  const Uint8* keyState = SDL_GetKeyboardState(NULL); // Get current state again

  Network::Message<Network::CustomMsgTypes> msg;
  msg.header.id = Network::CustomMsgTypes::Client_InputUpdate;

  Network::PlayerStateInput input;
  // Check keys relevant to Ship's InputComponent
  // Assuming default keys W, S, A, D, Space
  input.forward = keyState[SDL_SCANCODE_W];
  input.back = keyState[SDL_SCANCODE_S];
  input.left = keyState[SDL_SCANCODE_A];
  input.right = keyState[SDL_SCANCODE_D];
  input.fire = keyState[SDL_SCANCODE_SPACE];

  // Pack the data (reverse order of reading for easier debugging)
  msg << input.forward << input.back << input.left << input.right << input.fire;

  mClient.Send(msg);
}

void Game::UpdateGame()
{
    // Compute delta time
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();

  // Check connection status
  mIsConnected = mClient.IsConnected();

  // 1. Process incoming network messages
  if (mIsConnected) {
    ProcessNetworkMessages();
  } else {
    if (mPlayerID != 0) { // Check if we *were* connected
      std::cout << "[CLIENT] Disconnected from server." << std::endl;
      // Handle disconnection logic (e.g., remove other players, show message)
      for (auto const& [id, ship] : mOtherPlayerShips) {
        RemoveActor(ship); // This should trigger deletion
      }
      mOtherPlayerShips.clear();
      mPlayerID = 0; // Reset player ID
      // Maybe disable player input, etc.
      if (mPlayerShip) mPlayerShip->SetState(Actor::EPaused);
    }
  }

    // Update all actors
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // Move any pending actors to mActors
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // Add any dead actors to a temp vector
    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }

    // Delete dead actors (which removes them from mActors)
    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::ProcessNetworkMessages()
{
  while (!mClient.IncomingMessages().empty())
  {
    auto ownedMsg = mClient.IncomingMessages().pop_front();
    auto message = ownedMsg.message; // Get the message part
  }

  switch (message.header.id) {
    case Network::CustomMsgTypes::ServerAccept:
    {
      // Server approved connection and sent our ID
      message >> mPlayerID;
      std::cout << "[CLIENT] Server accepted connection. My ID: " << mPlayerID << std::endl;
      // Now we can fully activate the local player ship
      if (mPlayerShip) mPlayerShip->SetState(Actor::EActive);
      // Optionally, send a "Client_RequestWorld" message here if needed
    }
    break;

    case Network::CustomMsgTypes::ServerDeny:
    {
      std::cout << "[CLIENT] Server denied connection." << std::endl;
      mIsRunning = false; // Or handle differently
    }
    break;

    case Network::CustomMsgTypes::ServerPing:
    {
      // Handle ping response if implementing latency check
      // std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
      // std::chrono::system_clock::time_point timePrev;
      // message >> timePrev;
      // std::cout << "[CLIENT] Ping: " << std::chrono::duration<double>(timeNow - timePrev).count() << std::endl;
    }
    break;

    case Network::CustomMsgTypes::Game_WorldState:
    {
      uint32_t playerCount = 0;
      message >> playerCount;
      // std::cout << "World state received for " << playerCount << " players." << std::endl;

      std::unordered_map<uint32_t, bool> receivedPlayers; // Track players in this update

      for (uint32_t i = 0; i < playerCount; ++i) {
        Network::PlayerState state;
        // Read in reverse order of writing
        message >> state.playerID
                >> state.posX >> state.posY >> state.rotation
                >> state.velX >> state.velY // Read optional velocity
                >> state.animState // Read optional anim state
                >> state.isVisible; // Read visibility

        receivedPlayers[state.playerID] = true;

        if (state.playerID == mPlayerID) {
          // This is the state of our own ship according to the server.
          // --- Reconciliation Logic (Advanced) ---
          // Compare server state (state.posX, state.posY) with local state (mPlayerShip->GetPosition())
          // If they differ significantly, smoothly correct the local ship's position.
          // For now, we ignore the server's state for our own ship to avoid jitter.
          // We could update animation/visibility based on server though.
          if(mPlayerShip) {
            auto* animComp = mPlayerShip->GetComponent<AnimSpriteComponent>();
            if (animComp) {
              if (animComp->GetNumAnim() != state.animState) {
                animComp->SetNumAnim(state.animState);
              }
              if (animComp->IsVisible() != state.isVisible) {
                animComp->SetVisible(state.isVisible);
              }
            }
          }
        } else {
          // This is state for another player's ship
          auto it = mOtherPlayerShips.find(state.playerID);
          if (it != mOtherPlayerShips.end()) {
            // Ship exists, update its state (interpolation needed for smoothness)
            Ship* ship = it->second;
            ship->SetPosition(Vector2(state.posX, state.posY));
            ship->SetRotation(state.rotation);
            // Update animation / visibility
            auto* animComp = ship->GetComponent<AnimSpriteComponent>();
            if (animComp) {
              if (animComp->GetNumAnim() != state.animState) {
                animComp->SetNumAnim(state.animState);
              }
              if (animComp->IsVisible() != state.isVisible) {
                animComp->SetVisible(state.isVisible);
              }
            }
            // TODO: Update velocity in MoveComponent if needed for client-side interpolation

          } else {
            // Ship doesn't exist, create it
            std::cout << "[CLIENT] Spawning ship for player " << state.playerID << std::endl;
            Ship* newShip = new Ship(this); // Create a new ship actor
            newShip->SetPosition(Vector2(state.posX, state.posY));
            newShip->SetRotation(state.rotation);
            newShip->SetState(Actor::EActive); // Make it active

            // --- IMPORTANT: Disable input for remote ships ---
            // newShip->RemoveComponent(newShip->GetComponent<InputComponent>()); // Or disable it
            auto* inputComp = newShip->GetComponent<InputComponent>();
            if(inputComp) inputComp->SetEnabled(false); // Add SetEnabled to Component base class?

            // Update animation / visibility
            auto* animComp = newShip->GetComponent<AnimSpriteComponent>();
            if (animComp) {
              animComp->SetNumAnim(state.animState);
              animComp->SetVisible(state.isVisible);
            }

            mOtherPlayerShips[state.playerID] = newShip;
            // AddActor(newShip); // Already done by Actor constructor
          }
        }
      }

      // Check for players who were NOT in this update (disconnected)
      std::vector<uint32_t> disconnectedIDs;
      for (auto const& [id, ship] : mOtherPlayerShips) {
        if (receivedPlayers.find(id) == receivedPlayers.end()) {
          // This player wasn't in the update, assume disconnected
          disconnectedIDs.push_back(id);
        }
      }

      // Remove disconnected players
      for (uint32_t id : disconnectedIDs) {
        std::cout << "[CLIENT] Removing disconnected player " << id << std::endl;
        auto it = mOtherPlayerShips.find(id);
        if (it != mOtherPlayerShips.end()) {
          RemoveActor(it->second); // Marks for deletion in next update loop
          delete it->second; // Force deletion now? Be careful with game loop structure
          mOtherPlayerShips.erase(it);
        }
      }
    }
    break;
    case Network::CustomMsgTypes::Game_PlayerDisconnect:
    {
      uint32_t disconnectedID = 0;
      message >> disconnectedID;
      std::cout << "[CLIENT] Server announced player " << disconnectedID << " disconnected." << std::endl;
      auto it = mOtherPlayerShips.find(disconnectedID);
      if (it != mOtherPlayerShips.end()) {
        RemoveActor(it->second); // Marks for deletion
        // delete it->second; // Or delete immediately
        mOtherPlayerShips.erase(it);
      }
    }
    break;

    default:
      std::cout << "[CLIENT] Unhandled message type: " << static_cast<uint32_t>(message.header.id) << std::endl;
    break;

  }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
    SDL_RenderClear(mRenderer);

    // Draw all sprite components
    for (auto sprite : mSprites)
    {
        sprite->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
#if 0
  // Create player's ship
  // mShip = new Ship(this);
  // mShip->SetPosition(Vector2(100.0f, 384.0f));
  // mShip->SetScale(1.5f);

  // Create a Player
  mCharcter = new Character(this);
  mCharcter->SetPosition(Vector2(200.0f, 384.0f));
  // mCharcter->SetScale(1.5f);

  // Create a TileMap Background
  mBackground = new Background(this);
  mBackground->SetPosition(Vector2(512.0f, 384.0f));
  // mBackground->SetScale(0.5f);

  // Create actor for the background (this doesn't need a subclass)
  Actor* temp = new Actor(this);
  temp->SetPosition(Vector2(512.0f, 384.0f));
  // Create the "far back" background
  BGSpriteComponent* bg = new BGSpriteComponent(temp);
  bg->SetScreenSize(Vector2(1024.0f, 768.0f));
  std::vector<SDL_Texture*> bgtexs = {
      GetTexture("Assets/Farback01.png"),
      GetTexture("Assets/Farback02.png")
  };
  bg->SetBGTextures(bgtexs);
  bg->SetScrollSpeed(-100.0f);
  // Create the closer background
  bg = new BGSpriteComponent(temp, 50);
  bg->SetScreenSize(Vector2(1024.0f, 768.0f));
  bgtexs = {
      GetTexture("Assets/Stars.png"),
      GetTexture("Assets/Stars.png")
  };
  bg->SetBGTextures(bgtexs);
  bg->SetScrollSpeed(-200.0f);
#endif
    // Create player's ship
    mShip = new Ship(this);
    mShip->SetPosition(Vector2(512.0f, 384.0f));
    mShip->SetRotation(Math::PiOver2);

    // Create asteroids
    const int numAsteroids = 10;
    for (int i = 0; i < numAsteroids; i++)
    {
        new Asteroid(this);
    }
}

void Game::UnloadData()
{
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    // Destroy textures
    for (auto i : mTextures)
    {
        SDL_DestroyTexture(i.second);
    }
    mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
    SDL_Texture* tex = nullptr;
    // Is the texture already in the map?
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        // Load from file
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf)
        {
            SDL_Log("Failed to load texture file %s", fileName.c_str());
            return nullptr;
        }

        // Create texture from surface
        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex)
        {
            SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
            return nullptr;
        }

        mTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}

std::vector<std::string> Game::GetCSV(const std::string& fileName)
{
  std::ifstream iFile;
  std::string line = "";
  iFile.open(fileName);
  std::vector<std::string> csv;

  while (getline(iFile, line))
  {
    csv.push_back(line);
  }
  iFile.close();

  return csv;
}

void Game::AddAsteroid(Asteroid* ast)
{
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
    auto iter = std::find(mAsteroids.begin(),
                          mAsteroids.end(), ast);
    if (iter != mAsteroids.end())
    {
        mAsteroids.erase(iter);
    }
}

void Game::Shutdown()
{
  // Disconnect client before unloading data
  if (mClient.IsConnected()) {
    mClient.Disconnect();
  }

    UnloadData();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
    // If we're updating actors, need to add to pending
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    // Is it in pending actors?
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    // Is it in actors?
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddSprite(SpriteComponent* sprite)
{
    // Find the insertion point in the sorted vector
    // (The first element with a higher draw order than me)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for ( ;
            iter != mSprites.end();
    ++iter)
            {
                if (myDrawOrder < (*iter)->GetDrawOrder())
                {
                    break;
                }
            }

            // Inserts element before position of iterator
            mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
    // (We can't swap because it ruins ordering)
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

}
