#pragma once

#include "Actor.hpp"
#include "Component/ScreenComponent.hpp"

namespace EngineG{

enum class ScreenState {
  NONE,
  MAIN_MENU,
  LOBBY_HOST,
  LOBBY_CLIENT,
  GAME,
};

class ScreenManager: public Actor
{
public:
  ScreenManager(class Game* game);
  ~ScreenManager() = default;

  void AddScreen(ScreenState state, class ScreenComponent<ScreenState>* screen);
  void ChangeScreen(ScreenState newState);
  void RemoveScreen(ScreenState state) = delete;

protected:
  std::vector<class ScreenComponent<ScreenState>*> mScreens;
  ScreenComponent<ScreenState>* mCurScreen = nullptr;
  ScreenState mCurState = ScreenState::NONE;
};

}