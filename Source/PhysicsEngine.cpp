#include "include/WarpedRealms/PhysicsEngineDLL.hpp"

#include <thread>
#include <cui.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// #include <docopt/docopt.h>

#include <fmt/core.h>
#include <fmt/color.h>

#include <box2d/box2d.h>

// #include "adapter/AdapterClient.hpp"
// #include "adapter/AdapterServer.hpp"

#include <cmath>

float f_pi = M_PI;

// #include <box2cpp/box2cpp.h>
// #include <box2cpp/debug_imgui_renderer.h>
#include <GLFW/glfw3.h>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

#include <entt/entt.hpp>
#include "ExampleEnTT.hpp"
#include "engine/Engine.hpp"
#include "engine/Test.hpp"

namespace PhysicsEngine
{

std::thread Physics;
std::thread Console;

void PhysicsThread(Engine& engine)
{
  using namespace std::chrono_literals;

  while (engine.running.load())
  {
    {
      std::lock_guard<std::mutex> lock(engine.positionMutex);
      engine.x += 0.1;
      engine.y += 0.2;
      engine.z += 0.3;
      engine.tick++;
    }

    std::this_thread::sleep_for(100ms);
  }
}

Engine* engine = nullptr;

void StartPhysics()
{
  if(engine) return;

  engine = new Engine{};

  engine->running = true;
  Physics = std::thread(PhysicsThread, std::ref(*engine));
  // Console = std::thread(cui, std::ref(engine->running));
}

int GetObjectPosition(double* out_x, double* out_y, double* out_z)
{
  std::lock_guard<std::mutex> lock(engine->positionMutex);
  *out_x = engine->x;
  *out_y = engine->y;
  *out_z = engine->z;
  return engine->tick;
}

void StopPhysics()
{
  engine->running = false;

  Physics.join();
  // Console.join();

  delete engine;
  engine = nullptr;
}


void (*callbackPtr)(void) = nullptr;

void SetCallback(void (*ptr)(void)){
  callbackPtr = ptr;
}

void CallMeMaybe(void){
  callbackPtr();
}

bool IsLengthGreaterThan5(const char* value) {
  return std::strlen(value) > 5;
}

const char* GetName() {
  static const std::string name = "LinuxString";
  return name.c_str();
}


std::thread ServerThread;
std::thread ClientThread;

#if 0
int testWindowBox2dImgui()
{
  const int WIDTH = 800, HEIGHT = 600;

  if (!glfwInit()) return -1;
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Box2D + ImGui (box2cpp)", NULL, NULL);
  if (!window) return -1;
  glfwMakeContextCurrent(window);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  b2::World world(b2::World::Params{});
  b2::DebugImguiRenderer debug_renderer;

  b2::Body::Params bp;

  auto ground = world.CreateBody(b2::OwningHandle, bp);

  ground.CreateShape(
          b2::DestroyWithParent,
          b2::Shape::Params{},
          b2Circle{.center = b2Vec2(), .radius = 3}
      );

  b2::Body b = world.CreateBody(b2::OwningHandle, bp);

  b.CreateShape(
    b2::DestroyWithParent,
    b2::Shape::Params{},
    b2Circle{.center = b2Vec2(), .radius = 3}
    );

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    world.Step(1.0f / 60.0f, 4);

    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    debug_renderer.DrawShapes(world);
    debug_renderer.MouseDrag(world);
    debug_renderer.DrawModeToggles();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void StartServer()
{
  // some logic
  std::cout << "StartServer" << std::endl;

  ServerThread = std::thread(AdapterServer::startServer);
  ClientThread = std::thread(AdapterClient::startClient);

  ServerThread.join();
  ClientThread.join();
}
#endif

void StartGameEngine()
{
#if 0
  spdlog::info("Простое логирование");
  spdlog::warn("Предупреждение: {}", 42);
  spdlog::error("Ошибка! Код {}", -1);

  // Log with name
  auto logger = spdlog::stdout_color_mt("console");
  logger->info("Привет, {}!", "мир");

  // Log to file
  auto file_logger = spdlog::basic_logger_mt("file_logger", "logs.txt");
  file_logger->info("Записываем в файл");

  // Rotating logger
  auto rotating_logger = spdlog::rotating_logger_mt("rotating_logger", "logs_rotating.txt", 1024 * 1024, 3);
  rotating_logger->info("Ротация логов по размеру файла");

  // Async logging
  spdlog::init_thread_pool(8192, 1);
  auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_logger", "async_logs.txt");
  async_logger->info("Асинхронный лог");

  // Color logging
  auto color_logger = spdlog::stdout_color_mt("color_console");
  color_logger->info("Лог с цветами");
  color_logger->error("Ошибка!");

  // Set Level logging
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("Это сообщение будет видно только если уровень DEBUG включен");

  // FMT
  std::string name = "Alice";
  int age = 25;

  std::string result = fmt::format("Имя: {}, Возраст: {}", name, age);
  std::cout << result << std::endl;

  int num = 42;

  fmt::print("Обычное число: {}\n", num);
  fmt::print("Шестнадцатеричное: {:x}\n", num);  // hex
  fmt::print("Двоичное: {:b}\n", num);          // binary
  fmt::print("С плавающей точкой: {:.2f}\n", 3.1415926);

  fmt::print(fmt::fg(fmt::color::red), "Красный текст\n");
  fmt::print(fmt::fg(fmt::color::green), "Зеленый текст\n");
  fmt::print(fmt::fg(fmt::color::blue) | fmt::emphasis::bold, "Синий жирный текст\n");

  spdlog::info("StartGameEngine");

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture* texture = nullptr;
  SDL_Surface* surface = nullptr;

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("SDL3 Displaying Image", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 640,480,0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  surface = IMG_Load("res/images/surface.png");
  texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Event event;
  bool quit = false;

  SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  SDL_Delay(3000);

  while (!quit)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
    }
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();



#endif
  // b2WorldDef worldDef = b2DefaultWorldDef();
  // worldDef.gravity = (b2Vec2){0.0f, -10.0f};
  // b2WorldId worldId = b2CreateWorld(&worldDef);
  //
  // b2BodyDef groundBodyDef = b2DefaultBodyDef();
  // groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
  //
  // b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
  // b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
  //
  // b2ShapeDef groundShapeDef = b2DefaultShapeDef();
  // b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
  //
  // b2BodyDef bodyDef = b2DefaultBodyDef();
  // bodyDef.type = b2_dynamicBody;
  // bodyDef.position = (b2Vec2){0.0f, 4.0f};
  // b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
  //
  // float timeStep = 1.0f / 60.0f;
  // int subStepCount = 4;
  //
  // for (int i = 0; i < 90; ++i)
  // {
  //   b2World_Step(worldId, timeStep, subStepCount);
  //   b2Vec2 position = b2Body_GetPosition(bodyId);
  //   b2Rot rotation = b2Body_GetRotation(bodyId);
  //   printf("%4.2f %4.2f %4.2f\n", position.x, position.y, b2Rot_GetAngle(rotation));
  // }
  //
  // b2DestroyWorld(worldId);

  // testWindowBox2dImgui();

  Examples::startExampleEnTT();
  EngineG::Test::Run();
}




}  // namespace PhysicsEngine
