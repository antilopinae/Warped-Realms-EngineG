#include "Tile.hpp"

#include "../Game.hpp"
#include "Component/SpriteComponent.hpp"

namespace EngineG {

Tile::Tile(class Game* game)
    : Actor(game)
    , mParent(nullptr)
    , f(0.0f)
    , g(0.0f)
    , h(0.0f)
    , mBlocked(false)
    , mSprite(nullptr)
    , mTileState(EDefault)
    , mSelected(false) {
    mSprite = new SpriteComponent(this);
    UpdateTexture();
}

void Tile::SetTileState(TileState state) {
    mTileState = state;
    UpdateTexture();
}

void Tile::ToggleSelect() {
    mSelected = !mSelected;
    UpdateTexture();
}

void Tile::UpdateTexture() {
    std::string text;
    switch (mTileState) {
        case EStart:
            text = "TileTan.png";
            break;
        case EBase:
            text = "TileGreen.png";
            break;
        case EPath:
            if (mSelected)
                text = "TileGreySelected.png";
            else
                text = "TileGrey.png";
            break;
        case EDefault:
        default:
            if (mSelected)
                text = "TileBrownSelected.png";
            else
                text = "TileBrown.png";
            break;
    }
    mSprite->SetTexture(GetGame()->GetTexture(text));
}

}  // namespace EngineG
