#include <cereal/archives/json.hpp>

#include "Game.hpp"

#include "Actor.hpp"
#include "Math.hpp"
#include "Test/ExWasm.hpp"

#include <yojimbo.h>
#include <entt/entt.hpp>
// #include <box2d/box2d.h>
#include <sodium.h>

#include <fstream>
#include <iostream>

#include <imgui.h>
#include <imnodes.h>
#include <SDL.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Time.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/Platform/Sdl2Application.h>


#include <cmath>
constexpr float f_pi = M_PI;

#include <box2cpp/box2cpp.h>
#include <box2cpp/debug_imgui_renderer.h>

namespace Magnum { namespace Examples {

    using namespace Math::Literals;

    class ImGuiExample: public Platform::Application {
    public:
        explicit ImGuiExample(const Arguments& arguments);

        void drawEvent() override;

        void viewportEvent(ViewportEvent& event) override;

        void keyPressEvent(KeyEvent& event) override;
        void keyReleaseEvent(KeyEvent& event) override;

        void pointerPressEvent(PointerEvent& event) override;
        void pointerReleaseEvent(PointerEvent& event) override;
        void pointerMoveEvent(PointerMoveEvent& event) override;
        void scrollEvent(ScrollEvent& event) override;
        void textInputEvent(TextInputEvent& event) override;

        ~ImGuiExample(){ ImNodes::DestroyContext(); }

    private:
        ImGuiIntegration::Context _imgui{NoCreate};

        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
        Color4 _clearColor = 0x72909aff_rgbaf;
        Float _floatValue = 0.0f;

        b2::World world{b2::World::Params{}};
        b2::DebugImguiRenderer debug_renderer;

        b2::Body::Params bp;

        b2::Body ground;
        b2::Body b;
    };

    ImGuiExample::ImGuiExample(const Arguments& arguments): Platform::Application{arguments,
        Configuration{}.setTitle("Magnum ImGui Example")
        .setWindowFlags(Configuration::WindowFlag::Resizable)}
        {
            _imgui = ImGuiIntegration::Context(Vector2{windowSize()}/dpiScaling(),
                                               windowSize(), framebufferSize());

            /* Set up proper blending to be used by ImGui. There's a great chance
             *       you'll need this exact behavior for the rest of your scene. If not, set
             *       this only for the drawFrame() call. */
            GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
                                           GL::Renderer::BlendEquation::Add);
            GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
                                           GL::Renderer::BlendFunction::OneMinusSourceAlpha);

            #if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
            /* Have some sane speed, please */
            setMinimalLoopPeriod(16.0_msec);
            #endif

            ImNodes::CreateContext();

            ground = world.CreateBody(b2::OwningHandle, bp);

            ground.CreateShape(
                b2::DestroyWithParent,
                b2::Shape::Params{},
                b2Circle{.center = b2Vec2(), .radius = 3}
            );

            b = world.CreateBody(b2::OwningHandle, bp);

            b.CreateShape(
                b2::DestroyWithParent,
                b2::Shape::Params{},
                b2Circle{.center = b2Vec2(), .radius = 3}
            );
        }

        void ImGuiExample::drawEvent() {
            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

            _imgui.newFrame();

            /* Enable text input, if needed */
            if(ImGui::GetIO().WantTextInput && !isTextInputActive())
                startTextInput();
            else if(!ImGui::GetIO().WantTextInput && isTextInputActive())
                stopTextInput();

            /* 1. Show a simple window.
             *       Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appear in
             *       a window called "Debug" automatically */
            {
                ImGui::Text("Hello, world!");
                ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);
                if(ImGui::ColorEdit3("Clear Color", _clearColor.data()))
                    GL::Renderer::setClearColor(_clearColor);
                if(ImGui::Button("Test Window"))
                    _showDemoWindow ^= true;
                if(ImGui::Button("Another Window"))
                    _showAnotherWindow ^= true;
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0/Double(ImGui::GetIO().Framerate), Double(ImGui::GetIO().Framerate));

                debug_renderer.DrawShapes(world);
                debug_renderer.MouseDrag(world);
                debug_renderer.DrawModeToggles();
            }

            /* 2. Show another simple window, now using an explicit Begin/End pair */
            if(_showAnotherWindow) {
                ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
                ImGui::Begin("Another Window", &_showAnotherWindow);
                ImNodes::BeginNodeEditor();

                ImNodes::BeginNode(1);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("Node 1");
                ImNodes::EndNodeTitleBar();

                ImNodes::BeginInputAttribute(2);
                ImGui::Text("Input");
                ImNodes::EndInputAttribute();

                ImNodes::BeginOutputAttribute(3);
                ImGui::Text("Output");
                ImNodes::EndOutputAttribute();

                ImNodes::EndNode();

                ImNodes::EndNodeEditor();
                ImGui::End();
            }

            /* 3. Show the ImGui demo window. Most of the sample code is in
             *       ImGui::ShowDemoWindow() */
            if(_showDemoWindow) {
                ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
                ImGui::ShowDemoWindow();
            }

            /* Update application cursor */
            _imgui.updateApplicationCursor(*this);

            /* Set appropriate states. If you only draw ImGui, it is sufficient to
             *       just enable blending and scissor test in the constructor. */
            GL::Renderer::enable(GL::Renderer::Feature::Blending);
            GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
            GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
            GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

            _imgui.drawFrame();

            /* Reset state. Only needed if you want to draw something else with
             *       different state after. */
            GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
            GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
            GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
            GL::Renderer::disable(GL::Renderer::Feature::Blending);

            swapBuffers();
            redraw();
        }

        void ImGuiExample::viewportEvent(ViewportEvent& event) {
            GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

            _imgui.relayout(Vector2{event.windowSize()}/event.dpiScaling(),
                            event.windowSize(), event.framebufferSize());
        }

        void ImGuiExample::keyPressEvent(KeyEvent& event) {
            if(_imgui.handleKeyPressEvent(event)) return;
        }

        void ImGuiExample::keyReleaseEvent(KeyEvent& event) {
            if(_imgui.handleKeyReleaseEvent(event)) return;
        }

        void ImGuiExample::pointerPressEvent(PointerEvent& event) {
            if(_imgui.handlePointerPressEvent(event)) return;
        }

        void ImGuiExample::pointerReleaseEvent(PointerEvent& event) {
            if(_imgui.handlePointerReleaseEvent(event)) return;
        }

        void ImGuiExample::pointerMoveEvent(PointerMoveEvent& event) {
            if(_imgui.handlePointerMoveEvent(event)) return;
        }

        void ImGuiExample::scrollEvent(ScrollEvent& event) {
            if(_imgui.handleScrollEvent(event)) {
                /* Prevent scrolling the page */
                event.setAccepted();
                return;
            }
        }

        void ImGuiExample::textInputEvent(TextInputEvent& event) {
            if(_imgui.handleTextInputEvent(event)) return;
        }

}}

namespace EngineG {

Game::Game()
    : mWindow(nullptr)
    , mRenderer(nullptr)
    , mIsRunning(true)
    , mUpdatingActors(false)
    , mClient()
    , mServer(8000)
    , mWasmNetworkManager(nullptr)
    , mWasmIntegrationService() {
}

bool Game::Initialize() {
    if (sodium_init() < 0) return false;

    entt::registry reg;
    cereal::JSONOutputArchive archive(std::cout);

    b2Vec2 gravity{0.0f, -9.8f};

    const char* argv[] = {"program"};
    int i = 1;
    Magnum::Examples::ImGuiExample app({i, const_cast<char**>(argv)});
    app.exec();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1024, 768, 0);
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    Random::Init();
    mWasmNetworkManager = std::make_unique<Wasm::WasmNetworkManager>(this);

    LoadData();

    // if (mWasmNetworkManager) {
    //   Wasm::LogMessage("EngineG::Game::Initialize - Requesting initial WASM package...");
    //   mWasmNetworkManager->RequestAndApplyPackage("127.0.0.1", "8081", "behavior_pack_A", "Player_Actor_Main");
    // }

    // mServer.Start();
    // mClient.Connect("127.0.0.1", 8000);
    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

#if 0
  // Process ship input
  // mShip->ProcessKeyboard(state);
  mCharcter->ProcessKeyboard(state);
#endif

    mUpdatingActors = true;
    for (auto actor : mActors) {
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

void Game::UpdateGame() {
    // Compute delta time
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
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
            mPlayerID = 0;  // Reset player ID
        }
    }

    // Update all actors
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->Update(deltaTime);
    }

    mWasmIntegrationService.Update(deltaTime);

    mUpdatingActors = false;

    // Move any pending actors to mActors
    for (auto pending : mPendingActors) {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // Add any dead actors to a temp vector
    std::vector<Actor*> deadActors;
    for (auto actor : mActors) {
        if (actor->GetState() == Actor::EDead) {
            deadActors.emplace_back(actor);
        }
    }

    // Delete dead actors (which removes them from mActors)
    for (auto actor : deadActors) {
        delete actor;
    }
}

void Game::ProcessNetworkMessages() {
    while (!mClient.IncomingMessages().empty()) {
        auto ownedMsg = mClient.IncomingMessages().pop_front();
        auto message = ownedMsg.message;

        switch (message.header.id) {
            case Network::GameMsgTypes::ServerAccept: {
                auto accept = Network::ParseMsgServerAccept(message);
                mPlayerID = accept.clientID;
                std::cout << "[CLIENT] Server accepted connection. My ID: " << mPlayerID << std::endl;
                if (mShip) mShip->SetState(Actor::EActive);
            } break;

            case Network::GameMsgTypes::ServerPing: {
                auto ping = Network::ParseMsgServerPing(message);
                std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point timePrev{std::chrono::milliseconds(ping.timestamp)};
                std::cout << "[CLIENT] Ping: " << std::chrono::duration<double>(timeNow - timePrev).count() << std::endl;
            } break;

            case Network::GameMsgTypes::GamePlayerDisconnect: {
                uint32_t disconnectedID = 0;
                auto disc = Network::ParseMsgGamePlayerDisconnect(message);
                disconnectedID = disc.clientID;
                std::cout << "[CLIENT] Server announced player " << disconnectedID << " disconnected." << std::endl;
                auto it = mOtherShips.find(disconnectedID);
                if (it != mOtherShips.end()) {
                    RemoveActor(it->second);  // Marks for deletion
                    mOtherShips.erase(it);
                }
            } break;

            default:
                std::cout << "[CLIENT] Unhandled message type: " << static_cast<uint32_t>(message.header.id) << std::endl;
                break;
        }
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
    SDL_RenderClear(mRenderer);

    // Draw all sprite components
    for (auto sprite : mSprites) {
        sprite->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
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
    for (int i = 0; i < numAsteroids; i++) {
        new Asteroid(this);
    }
}

void Game::UnloadData() {
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Destroy textures
    for (auto i : mTextures) {
        SDL_DestroyTexture(i.second);
    }
    mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
    SDL_Texture* tex = nullptr;
    // Is the texture already in the map?
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end()) {
        tex = iter->second;
    } else {
        // Load from file
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf) {
            SDL_Log("Failed to load texture file %s", fileName.c_str());
            return nullptr;
        }

        // Create texture from surface
        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex) {
            SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
            return nullptr;
        }

        mTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}

std::vector<std::string> Game::GetCSV(const std::string& fileName) {
    std::ifstream iFile;
    std::string line = "";
    iFile.open(fileName);
    std::vector<std::string> csv;

    while (getline(iFile, line)) {
        csv.push_back(line);
    }
    iFile.close();

    return csv;
}

void Game::AddAsteroid(Asteroid* ast) {
    mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast) {
    auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
    if (iter != mAsteroids.end()) {
        mAsteroids.erase(iter);
    }
}

void Game::Shutdown() {
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

void Game::AddActor(Actor* actor) {
    // If we're updating actors, need to add to pending
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    } else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor) {
    // Is it in pending actors?
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    // Is it in actors?
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddSprite(SpriteComponent* sprite) {
    // Find the insertion point in the sorted vector
    // (The first element with a higher draw order than me)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for (; iter != mSprites.end(); ++iter) {
        if (myDrawOrder < (*iter)->GetDrawOrder()) {
            break;
        }
    }

    // Inserts element before position of iterator
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
    // (We can't swap because it ruins ordering)
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

Actor* Game::FindActorById(const std::string& actorId) {
    for (Actor* actor : mActors) {
        if (actor && actor->GetUniqueActorIdAsString() == actorId) {
            return actor;
        }
    }
    for (Actor* actor : mPendingActors) {
        if (actor && actor->GetUniqueActorIdAsString() == actorId) {
            return actor;
        }
    }
    Wasm::LogMessage("EngineG::Game::FindActorById - Actor with ID '" + actorId + "' not found.");
    return nullptr;
}

bool Game::ApplyWasmToActorComponent(const std::string& targetActorId,
                                     const std::string& scriptSlotNameOnComponent,
                                     const std::string& localWasmPath) {
    Wasm::LogMessage("EngineG::Game: ApplyWasmToActorComponent for ActorID: " + targetActorId +
                     ", Slot: " + scriptSlotNameOnComponent + ", Path: " + localWasmPath);
    Actor* actor = FindActorById(targetActorId);
    if (!actor) {
        Wasm::LogMessage("EngineG::Game ERROR: Target actor '" + targetActorId + "' not found.");
        return false;
    }

    Component* componentToHost = actor->GetComponentToHostWasmScript(scriptSlotNameOnComponent);
    if (!componentToHost) {
        Wasm::LogMessage("EngineG::Game ERROR: Actor '" + targetActorId + "' does not have suitable component for WASM slot '" +
                         scriptSlotNameOnComponent + "'.");
        return false;
    }

    Wasm::LogMessage("EngineG::Game: Found component on actor " + targetActorId + ". Requesting it to attach WASM script...");

    return componentToHost->AttachWasmScriptToSelf(scriptSlotNameOnComponent, localWasmPath);
}

}  // namespace EngineG
