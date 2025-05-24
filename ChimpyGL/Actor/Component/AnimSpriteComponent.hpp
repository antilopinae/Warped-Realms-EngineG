#pragma once

#include <vector>

#include "SpriteComponent.hpp"

namespace EngineG
{

class AnimSpriteComponent : public SpriteComponent
{
  public:
  struct Animation
  {
    Animation(int start, int end, bool loop = true) : mStart(start), mEnd(end), mLoop(loop), size(end - start + 1) {}

    int mStart;
    int mEnd;
    bool mLoop;
    float size;
  };

  enum State
  {
    ESingle,
    EMultiple
  };

  AnimSpriteComponent(Actor* owner, int drawOrder = 100);
  // Update animation every frame (overridden from component)
  void Update(float deltaTime) override;

  void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
  void SetAnimRanges(const std::vector<Animation>& ranges);

  State GetState() const
  {
    return mState;
  }
  int GetNumAnim() const
  {
    return mCurrAnim;
  }
  void SetNumAnim(int nextAnimation);

  float GetAnimFPS() const
  {
    return mAnimFPS;
  }
  void SetAnimFPS(float fps)
  {
    mAnimFPS = fps;
  }

  private:
  std::vector<Animation> mAnimRanges;
  State mState;
  // All textures in the animation
  std::vector<SDL_Texture*> mAnimTextures;
  // Current frame displayed
  float mCurrFrame;
  // Animation frame rate
  float mAnimFPS;
  // Current animation number
  int mCurrAnim;
};

}    // namespace EngineG
