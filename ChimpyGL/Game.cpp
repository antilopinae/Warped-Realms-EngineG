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
,mClient()
,mServer(80000)
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

    mServer.Start();
    mClient.Connect("127.0.0.1", 80000);

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

void Game::SendInputToServer(const Uint8* keyState) {
  Network::PlayerInput input;
  input.forward = keyState[SDL_SCANCODE_W];
  input.back = keyState[SDL_SCANCODE_S];
  input.left = keyState[SDL_SCANCODE_A];
  input.right = keyState[SDL_SCANCODE_D];
  input.fire = keyState[SDL_SCANCODE_SPACE];

  auto msg = Network::CreateMsgClientInputUpdate(input);

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

    mIsConnected = mClient.IsConnected();

    if (mIsConnected) {
      ProcessNetworkMessages();
    } else {
      if (mPlayerID != 0) {
        std::cout << "[CLIENT] Disconnected from server." << std::endl;
        mPlayerID = 0;
        if (mShip) mShip->SetState(Actor::EPaused);

        // Handle disconnection logic
        for (auto const& [id, ship] : mOtherShips) {
          RemoveActor(ship);
        }
        mOtherShips.clear();
        mPlayerID = 0; // Reset player ID
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
    auto message = ownedMsg.message;

    switch (message.header.id) {
      case Network::GameMsgTypes::ServerAccept:
      {
        auto accept = Network::ParseMsgServerAccept(message);
        mPlayerID = accept.clientID;
        std::cout << "[CLIENT] Server accepted connection. My ID: " << mPlayerID << std::endl;
        if (mShip) mShip->SetState(Actor::EActive);
      }
      break;

      case Network::GameMsgTypes::ServerPing:
      {
        auto ping = Network::ParseMsgServerPing(message);
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point timePrev {std::chrono::milliseconds(ping.timestamp)};
        std::cout << "[CLIENT] Ping: " << std::chrono::duration<double>(timeNow - timePrev).count() << std::endl;
      }
      break;

      case Network::GameMsgTypes::GamePlayerDisconnect:
      {
        uint32_t disconnectedID = 0;
        auto disc = Network::ParseMsgGamePlayerDisconnect(message);
        disconnectedID = disc.clientID;
        std::cout << "[CLIENT] Server announced player " << disconnectedID << " disconnected." << std::endl;
        auto it = mOtherShips.find(disconnectedID);
        if (it != mOtherShips.end()) {
          RemoveActor(it->second); // Marks for deletion
          mOtherShips.erase(it);
        }
      }
      break;

      default:
        std::cout << "[CLIENT] Unhandled message type: " << static_cast<uint32_t>(message.header.id) << std::endl;
      break;
    }
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
