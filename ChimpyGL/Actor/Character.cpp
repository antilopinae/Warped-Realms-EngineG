#include "Character.hpp"
#include "Component/AnimSpriteComponent.hpp"
#include "Component/Game.hpp"
#include <iomanip>
#include <string>

namespace EngineG{

Character::Character(class Game *game): Actor(game), speed(0.f)
{
  // Create an animated sprite component
  animSprite = new AnimSpriteComponent(this);
  std::vector<SDL_Texture*> anims = {};

  for (int i = 1; i < 19; i++)
  {
    std::ostringstream oss;
    oss << ASSETS_DIR"Assets/Character" << std::setw(2) << std::setfill('0') << i << ".png";
    std::string path = oss.str();

    anims.push_back(game->GetTexture(path));
  }

  std::vector<AnimSpriteComponent::Animation> animRanges = {
    {1, 6, true}, // walk
    {7, 15, false}, // jump
    {16, 18, false} // punch
  };

  animSprite->SetAnimTextures(anims);
  animSprite->SetAnimRanges(animRanges);
  // animSprite->SetNumAnim(3);
}

void Character::UpdateActor(float deltaTime)
{
  Actor::UpdateActor(deltaTime);
  Vector2 pos = GetPosition();
  pos.x += speed * deltaTime;

  if (Abs(speed) > 0.0f)
  {
    animSprite->SetNumAnim(0);
    if (speed < 0.0f) animSprite->SetReversed(true);
    else animSprite->SetReversed(false);
  }
  else animSprite->SetNumAnim(1);

  if (pos.x < 25.0f)
  {
    pos.x = 25.0f;
  }
  else if (pos.x > 975.0f)
  {
    pos.x = 975.0f;
  }
  SetPosition(pos);
}

void Character::ProcessKeyboard(const uint8_t* state)
{
  speed = 0.0f;
  // right/left
  if (state[SDL_SCANCODE_D])
  {
    speed += 250.0f;
  }
  if (state[SDL_SCANCODE_A])
  {
    speed -= 250.0f;
  }
}

}

