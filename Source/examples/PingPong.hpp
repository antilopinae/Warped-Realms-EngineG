#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "math/Math.hpp"

namespace EngineG{

using namespace Math;

class PingPong
{
public:
  PingPong();
  bool Initialize();
  // Runs the game loop until the game is over
  void RunLoop();
  // Shutdown the game
  void Shutdown();
private:
  void ProcessInput();
  void UpdatePingPong();
  void GenerateOutput();

  SDL_Window* mWindow;
  SDL_Renderer* mRenderer;
  Uint32 mTicksCount;
  bool mIsRunning;

  int mFirstPaddleDir;
  int mSecondPaddleDir;

  Vector2 mFirstPaddlePos;
  Vector2 mSecondPaddlePos;

  struct Ball
  {
    Vector2 pos;
    Vector2 vel;
  };

  std::vector<Ball> balls;
};


const int thickness = 15;
const float paddleH = 100.0f;


PingPong::PingPong()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mFirstPaddleDir(0)
,mSecondPaddleDir(0)
,balls()
{
}


bool PingPong::Initialize()
{
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow(
          "PingPong Programming in C++",
          100,
          100,
          1024,
          768,
          0
  );

  if (!mWindow)
  {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(
          mWindow,
          -1,		 //  which graphics driver to use
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (!mRenderer)
  {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  mFirstPaddlePos.x = 10.0f;
  mFirstPaddlePos.y = 768.0f/2.0f;

  mSecondPaddlePos.x = 1024.0f- 10.0f - paddleH/2;
  mSecondPaddlePos.y = 768.0f/2.0f;

  balls.push_back(Ball{
    .pos = {1024.0f/2.0f, 768.0f/2.0f },
    .vel = {-200.0f, 235.0f}
  });

  balls.push_back(Ball{
    .pos = {1024.0f/2.0f + 100.0f, 768.0f/2.0f },
    .vel = {200.0f, 235.0f}
  });

  return true;
}


void PingPong::RunLoop()
{
  while (mIsRunning)
  {
    ProcessInput();
    UpdatePingPong();
    GenerateOutput();
  }
}


void PingPong::ProcessInput()
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

  // Get state of keyboard
  const Uint8* state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_ESCAPE])
  {
    mIsRunning = false;
  }
  // Update paddle direction based on W/S keys
  mFirstPaddleDir = 0;
  if (state[SDL_SCANCODE_W])
  {
    mFirstPaddleDir -= 1;
  }
  if (state[SDL_SCANCODE_S])
  {
    mFirstPaddleDir += 1;
  }

  mSecondPaddleDir = 0;
  if (state[SDL_SCANCODE_I])
  {
    mSecondPaddleDir -= 1;
  }

  if (state[SDL_SCANCODE_K])
  {
    mSecondPaddleDir += 1;
  }
}


void PingPong::UpdatePingPong()
{
  // Wait until 16ms has elapsed since last frame
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  // Delta time is the difference in ticks from last frame
  // (converted to seconds)
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

  // Clamp maximum delta time value
  if (deltaTime > 0.05f)
  {
    deltaTime = 0.05f;
  }

  // Update tick counts (for next frame)
  mTicksCount = SDL_GetTicks();

  // Update paddle position based on direction
  if (mFirstPaddleDir != 0)
  {
    mFirstPaddlePos.y += mFirstPaddleDir * 300.0f * deltaTime;

    if (mFirstPaddlePos.y < (paddleH/2.0f + thickness))
    {
      mFirstPaddlePos.y = paddleH/2.0f + thickness;
    }
    else if (mFirstPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
    {
      mFirstPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
    }
  }

  if (mSecondPaddleDir != 0)
  {
    mSecondPaddlePos.y += mSecondPaddleDir * 300.0f * deltaTime;

    if (mSecondPaddlePos.y < (paddleH/2.0f + thickness))
    {
      mSecondPaddlePos.y = paddleH/2.0f + thickness;
    }
    else if (mSecondPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
    {
      mSecondPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
    }
  }

  for (auto& ball: balls)
  {
    ball.pos.x += ball.vel.x * deltaTime;
    ball.pos.y += ball.vel.y * deltaTime;

    float diff = mFirstPaddlePos.y - ball.pos.y;
    diff = (diff > 0.0f) ? diff : -diff;
    if (
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            ball.pos.x <= 25.0f && ball.pos.x >= 20.0f &&
            ball.vel.x < 0.0f)
    {
      ball.vel.x *= -1.0f;
    }
    // Did the ball go off the screen? (if so, end game)
    else if (ball.pos.x <= 0.0f)
    {
      mIsRunning = false;
    }
    // // Did the ball collide with the right wall?
    // else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
    // {
    //   mBallVel.x *= -1.0f;
    // }

    // Right Paddle
    diff = mSecondPaddlePos.y - ball.pos.y;
    diff = Mathf.Abs(diff);
    if (
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            ball.pos.x >= 1024 - 25.0f - paddleH/2 && ball.pos.x <= 1024 - 20.0f - paddleH/2 &&
            ball.vel.x > 0.0f)
    {
      ball.vel.x *= -1.0f;
    }
    // Did the ball go off the screen? (if so, end game)
    else if (ball.pos.x >= 1024.0f - 20.0f - paddleH/2)
    {
      mIsRunning = false;
    }


    // Did the ball collide with the top wall?
    if (ball.pos.y <= thickness && ball.vel.y < 0.0f)
    {
      ball.vel.y *= -1;
    }
    // Did the ball collide with the bottom wall?
    else if (ball.pos.y >= (768 - thickness) &&
            ball.vel.y > 0.0f)
    {
      ball.vel.y *= -1.0f;
    }
  }
}


void PingPong::GenerateOutput()
{
  // Set draw color to blue
  SDL_SetRenderDrawColor(
          mRenderer,
          0,		// R
          0,		// G
          255,	        // B
          255		// A
  );

  // Clear back buffer
  SDL_RenderClear(mRenderer);

  // Draw walls
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  // Draw top wall
  SDL_Rect wall{
    0,			// Top left x
    0,			// Top left y
    1024,		// Width
    thickness	        // Height
  };
  SDL_RenderFillRect(mRenderer, &wall);

  // Draw bottom wall
  wall.y = 768 - thickness;
  SDL_RenderFillRect(mRenderer, &wall);

  // Draw right wall
  // wall.x = 1024 - thickness;
  // wall.y = 0;
  // wall.w = thickness;
  // wall.h = 1024;
  // SDL_RenderFillRect(mRenderer, &wall);

  // top-left point
  SDL_Rect paddle{
    static_cast<int>(mFirstPaddlePos.x),
    static_cast<int>(mFirstPaddlePos.y - paddleH/2),
    thickness,
    static_cast<int>(paddleH)
  };
  SDL_RenderFillRect(mRenderer, &paddle);

  paddle.x = static_cast<int>(mSecondPaddlePos.x);
  paddle.y = static_cast<int>(mSecondPaddlePos.y - paddleH/2);
  paddle.w = thickness;
  paddle.h = static_cast<int>(paddleH);
  SDL_RenderFillRect(mRenderer, &paddle);

  // Draw ball
  SDL_Rect ballRender;

  for (const auto& ball: balls)
  {
    ballRender.x = static_cast<int>(ball.pos.x - thickness/2);
    ballRender.y = static_cast<int>(ball.pos.y - thickness/2);
    ballRender.w = thickness;
    ballRender.h = thickness;
    SDL_RenderFillRect(mRenderer, &ballRender);
  }
  // Draw ball
  // SDL_Rect ball{
  //   static_cast<int>(mBallPos.x - thickness/2),
  //   static_cast<int>(mBallPos.y - thickness/2),
  //   thickness,
  //   thickness
  // };
  // SDL_RenderFillRect(mRenderer, &ball);

  // Swap front buffer and back buffer
  SDL_RenderPresent(mRenderer);
}


void PingPong::Shutdown()
{
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}




}
