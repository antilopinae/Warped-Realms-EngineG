#pragma once

#include <entt/entt.hpp>

namespace Examples {

struct position {
  float x;
  float y;
};

struct velocity {
  float dx;
  float dy;
};

void update(entt::registry &registry) {
  auto view = registry.view<const position, velocity>();

  // view.each([](const auto &pos, auto &vel) { std::cout << pos.x << ", " << pos.y << std::endl; });

  view.each([](const auto entity, const auto &pos, auto &vel) { std::cout << pos.x << ", " << pos.y << std::endl; });

  // for(auto [entity, pos, vel]: view.each()) {}

  // for(auto entity: view) {
  //   auto &vel = view.get<velocity>(entity);
  // }
}

void startExampleEnTT() {
  entt::registry registry;

  for (auto i = 0u; i < 10u; ++i) {
    const auto entity = registry.create();
    registry.emplace<position>(entity, i * 1.f, i * 1.f);
    if (i % 2 == 0) {
      registry.emplace<velocity>(entity, i * .1f, i * .1f);
    }
  }

  update(registry);
}

}  // namespace Examples