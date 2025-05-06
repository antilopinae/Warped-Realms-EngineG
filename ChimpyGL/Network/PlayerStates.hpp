#pragma once

#include <cstdint>
#include <type_traits>

namespace EngineG::Network{

struct PlayerInput {
  bool forward = false;
  bool back = false;
  bool left = false;
  bool right = false;
  bool fire = false;
};

static_assert(std::is_standard_layout<PlayerInput>::value, "PlayerInput must be standard layout");

struct PlayerState {
  uint32_t playerID = 0;
  float posX = 0.0f;
  float posY = 0.0f;
  float rotation = 0.0f;
  float velX = 0.0f;
  float velY = 0.0f;
};

static_assert(std::is_standard_layout<PlayerState>::value, "PlayerState must be standard layout");

}