#pragma once

#include "Component.hpp"
#include <SDL2/SDL.h>

namespace EngineG {

class SpriteComponent : public Component {
public:
    // (Lower draw order corresponds with further back)
    SpriteComponent(Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(SDL_Renderer* renderer);
    virtual void SetTexture(SDL_Texture* texture);

    void SetReversed(bool reversed) { isReversed = reversed; }
    void SetVisible(bool visible) { isDraw = visible; }

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }
    bool IsReversed() const { return isReversed; }
    bool IsVisible() const { return isDraw; }

protected:
    SDL_Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
    bool isReversed;
    bool isDraw;
};

}  // namespace EngineG
