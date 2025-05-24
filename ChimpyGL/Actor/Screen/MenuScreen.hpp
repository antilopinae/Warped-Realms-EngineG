#pragma once

#include "../Component/ScreenComponent.hpp"
#include "../Component/SDL.hpp"

namespace EngineG
{

class MainMenuScreen : public ScreenComponent<ScreenState> {
public:
    MainMenuScreen(SDL_Renderer* renderer);
    ~MainMenuScreen();

private:
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
    void onHostClicked();
    void onConnectClicked();

    SDL_Renderer* localRenderer;

    SDL_Rect ipInputRect;
    std::string ipAddressText;
    bool ipInputActive = false;
    SDL_Texture* ipInputTexture = nullptr;

    std::string ipToConnect;
};

}
