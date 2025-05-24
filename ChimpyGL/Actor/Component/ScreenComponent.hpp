#pragma once

#include "SpriteComponent.hpp"

namespace EngineG
{

template <typename T>
class ScreenComponent : public SpriteComponent
{
  public:
  // (Lower draw order corresponds with further back)
  ScreenComponent(Actor* owner, int drawOrder = 100);
  ~ScreenComponent() = default;

  virtual void Draw(SDL_Renderer* renderer);

  virtual void EnterScreen() = 0;
  virtual void ExitScreen() = 0;

  T GetType() const
  {
    return mType;
  }

  protected:
  T mType;
};

}    // namespace EngineG