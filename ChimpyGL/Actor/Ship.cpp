#include "Ship.hpp"

#include "Component/AnimSpriteComponent.hpp"
#include "Component/Game.hpp"
#include "Component/InputComponent.hpp"
#include "Component/CircleComponent.hpp"
#include "Component/PhysicsInputComponent.hpp"
#include "Laser.hpp"
#include "Asteroid.hpp"

#include <iostream>

namespace EngineG{

Ship::Ship(class Game* game)
:Actor(game)
,mRightSpeed(0.0f)
,mDownSpeed(0.0f)
,mLaserCooldown(0.0f)
,isVisible(true)
{
  // Create an animated sprite component
  mAnimSprite = new AnimSpriteComponent(this);
  std::vector<SDL_Texture*> anims = {
#if 0
      game->GetTexture("Assets/Ship01.png"),
      game->GetTexture("Assets/Ship02.png"),
      game->GetTexture("Assets/Ship03.png"),
      game->GetTexture("Assets/Ship04.png"),
#endif
    game->GetTexture(ASSETS_DIR"Assets/Ship.png"),
    game->GetTexture(ASSETS_DIR"Assets/ShipWithThrust.png")
  };
  mAnimSprite->SetAnimTextures(anims);
  mAnimSprite->SetAnimRanges({
    AnimSpriteComponent::Animation(1,1, false),
    AnimSpriteComponent::Animation(2,2, false)
  });
  mAnimSprite->SetNumAnim(1);

  // Create an input component and set keys/speed
  // mInputComponent = new PhysicsInputComponent(this);
  mInputComponent = new InputComponent(this);
  mInputComponent->SetForwardKey(SDL_SCANCODE_W);
  mInputComponent->SetBackKey(SDL_SCANCODE_S);
  mInputComponent->SetClockwiseKey(SDL_SCANCODE_A);
  mInputComponent->SetCounterClockwiseKey(SDL_SCANCODE_D);
  mInputComponent->SetMaxForwardSpeed(300.0f);
  mInputComponent->SetMaxAngularSpeed(Math::TwoPi);

  // Create a circle component (for collision)
  mCircle = new CircleComponent(this);
  mCircle->SetRadius(8.0f);
}

void Ship::UpdateActor(float deltaTime)
{
  mLaserCooldown -= deltaTime;
  mDisappearCooldown -= deltaTime;

  if (mDisappearCooldown <= 0.3f)
  {
    if (!isVisible)
    {
      isVisible = true;
      mAnimSprite->SetVisible(true);
      mInputComponent->SetMaxForwardSpeed(300.0f);
      mInputComponent->SetMaxAngularSpeed(Math::TwoPi);
    }
  }
#if 0
    Actor::UpdateActor(deltaTime);
    // Update position based on speeds and delta time
    Vector2 pos = GetPosition();
    pos.x += mRightSpeed * deltaTime;
    pos.y += mDownSpeed * deltaTime;
    // Restrict position to left half of screen
    if (pos.x < 25.0f)
    {
        pos.x = 25.0f;
    }
    else if (pos.x > 500.0f)
    {
        pos.x = 500.0f;
    }
    if (pos.y < 25.0f)
    {
        pos.y = 25.0f;
    }
    else if (pos.y > 743.0f)
    {
        pos.y = 743.0f;
    }
    SetPosition(pos);
#endif
}


void Ship::ActorInput(const uint8_t* keyState)
{
  if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f && mDisappearCooldown <= 0.0f)
  {
      // Create a laser and set its position/rotation to mine
      Laser* laser = new Laser(GetGame());
      laser->SetPosition(GetPosition());
      laser->SetRotation(GetRotation());

      // Reset laser cooldown (half second)
      mLaserCooldown = 0.5f;
  }

  if (mDisappearCooldown <= 0.0f)
  {
    // Do we intersect with an asteroid?
    for (auto ast : GetGame()->GetAsteroids())
    {
      if (Intersect(*mCircle, *(ast->GetCircle())))
      {
        SetPosition(Vector2(512.0f, 384.0f));
        SetRotation(0.0f);
        std::cout << "Collide!" << std::endl;
        mDisappearCooldown = 1.0f;
        isVisible = false;
        mAnimSprite->SetVisible(false);
        mInputComponent->SetMaxForwardSpeed(0.0f);
        mInputComponent->SetMaxAngularSpeed(0.0f);
        // mInputComponent->SetVelocity(Vector2::Zero);
        break;
      }
    }
  }

// #if 0
  if (
  !Math::NearZero(mInputComponent->GetForwardSpeed()) ||
  !Math::NearZero(mInputComponent->GetAngularSpeed())
  )
  {
    if (mAnimSprite->GetNumAnim() == 0)
    {
      // Moving animation
      mAnimSprite->SetNumAnim(1);
    }
  }
  else
  {
    if (mAnimSprite->GetNumAnim() == 1)
    {
      // Idle animation
      mAnimSprite->SetNumAnim(0);
    }
  }
// #endif
}


#if 0
void Ship::ProcessKeyboard(const uint8_t* state)
{
    mRightSpeed = 0.0f;
    mDownSpeed = 0.0f;
    // right/left
    if (state[SDL_SCANCODE_D])
    {
        mRightSpeed += 250.0f;
    }
    if (state[SDL_SCANCODE_A])
    {
        mRightSpeed -= 250.0f;
    }
    // up/down
    if (state[SDL_SCANCODE_S])
    {
        mDownSpeed += 300.0f;
    }
    if (state[SDL_SCANCODE_W])
    {
        mDownSpeed -= 300.0f;
    }
}
#endif


}
