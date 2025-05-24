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

//     EngineG::Wasm::WasmBridge jsBridge;
//
//     if (!jsBridge.Initialize()) {
//       std::cerr << "Failed to initialize HybridBridge." << std::endl;
//     }
//
//     if (!jsBridge.RegisterNativeFunction("cpp_runWasmAdd", EngineG::Wasm::WasmBridge::NativeRunWasmAdd, 3)) {
//       std::cerr << "Failed to register native WASM runner function." << std::endl;
//       jsBridge.Shutdown();
//     }
//
//     std::vector<uint8_t> wasmFileBytes = jsBridge.ReadFileBytes(WASM_FILE_PATH);
//
//     if (wasmFileBytes.empty()) {
//       std::cerr << "WASM file is empty or could not be read: " << WASM_FILE_PATH << std::endl;
//       jsBridge.Shutdown();
//     }
//
//     JSValue jsWasmArrayBuffer = jsBridge.CreateArrayBufferFromBytes(wasmFileBytes);
//     if (JS_IsException(jsWasmArrayBuffer)) {
//       std::cerr << "Failed to create JS ArrayBuffer for WASM bytes." << std::endl;
//       JS_FreeValue(jsBridge.GetJsContext(), jsWasmArrayBuffer);
//       jsBridge.Shutdown();
//     }
//
//     JSContext* ctx = jsBridge.GetJsContext();
//     JSValue globalObj = JS_GetGlobalObject(ctx);
//
//     JS_SetPropertyStr(ctx, globalObj, "wasmBytesForJs", JS_DupValue(ctx, jsWasmArrayBuffer));
//     JS_FreeValue(ctx, globalObj);
//
//     const std::string js_code = R"javascript(
//         try {
//             console.log("[JS] Preparing to call cpp_runWasmAdd...");
//             // wasmBytesForJs был установлен из C++
//             if (typeof wasmBytesForJs === 'undefined') {
//                 throw new Error("wasmBytesForJs is not defined in JS global scope!");
//             }
//
//             let arg1 = 200;
//             let arg2 = 33;
//             console.log("[JS] Calling cpp_runWasmAdd(wasmBytesForJs, " + arg1 + ", " + arg2 + ")");
//
//             let sum = cpp_runWasmAdd(wasmBytesForJs, arg1, arg2); // arg1 и arg2 не используются WASM, но JS их передает
//             console.log("[JS] Result from cpp_runWasmAdd:", sum);
//
//             sum; // Это значение будет результатом JS_Eval
//         } catch (e) {
//             console.log("[JS] Error during script execution:", e.toString());
//             if (e.stack) {
//                 console.log("[JS] Stack: " + e.stack);
//             }
//             -1; // Вернуть -1 в случае ошибки в JS
//         }
//     )javascript";
//
//     std::cout << std::endl << "[Main] Evaluating JS code..." << std::endl;
//     JSValue result = jsBridge.ExecuteJsString(js_code, "mainWasmTestScript");
//
//     if (!JS_IsException(result)) {
//         std::cout << "[Main] JS evaluation successful." << std::endl;
//         int32_t c_result;
//         // Используем контекст, полученный от jsBridge
//         if (JS_ToInt32(ctx, &c_result, result) == 0) { // 0 - успех
//             std::cout << "[Main] Result from JS (should be sum 200+33): " << c_result << std::endl;
//         } else {
//           std::string type_name = "unknown";
//           int tag = JS_VALUE_GET_TAG(result); // Получаем тег значения
//
//           switch (tag) {
//             case JS_TAG_UNDEFINED: type_name = "undefined"; break;
//             case JS_TAG_NULL:      type_name = "null"; break;
//             case JS_TAG_BOOL:      type_name = "boolean"; break;
//             case JS_TAG_INT:       type_name = "integer"; break;
//             case JS_TAG_FLOAT64:   type_name = "float64"; break;
//             case JS_TAG_STRING:    type_name = "string"; break;
//             case JS_TAG_OBJECT:    type_name = "object"; break;
//             case JS_TAG_FUNCTION_BYTECODE: type_name = "function_bytecode"; break;
//             case JS_TAG_MODULE:    type_name = "module"; break;
//             case JS_TAG_BIG_INT:   type_name = "bigint"; break;
//             case JS_TAG_SYMBOL:    type_name = "symbol"; break;
//             default:
//               if (JS_IsObject(result)) type_name = "object (other)";
//               else type_name = "unknown_tag_" + std::to_string(tag);
//             break;
//           }
//
//           std::cout << "[Main] Could not convert JS result (type: " << type_name << ") to int." << std::endl;
//         }
//     } else {
//         std::cerr << "[Main] JS evaluation resulted in an exception (already printed by HybridBridge)." << std::endl;
//     }
//
//     JS_FreeValue(ctx, result);
//     JS_FreeValue(ctx, jsWasmArrayBuffer);
//
//     jsBridge.Shutdown();

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
