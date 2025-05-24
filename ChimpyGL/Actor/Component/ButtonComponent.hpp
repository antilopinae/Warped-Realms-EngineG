#pragma once

namespace EngineG {

struct Button {
    SDL_Rect rect;
    std::string text;
    SDL_Color color = {200, 200, 200, 255};
    SDL_Color hoverColor = {255, 255, 255, 255};
    SDL_Color textColor = {0, 0, 0, 255};
    bool hovered = false;
    void (*onClick)() = nullptr;                  // Простой обработчик
    ScreenState targetState = ScreenState::NONE;  // Для смены экрана через ScreenManager
};

}  // namespace EngineG
