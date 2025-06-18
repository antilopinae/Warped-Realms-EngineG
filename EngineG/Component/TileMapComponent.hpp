#pragma once

#include "SpriteComponent.hpp"
#include <string>
#include <vector>

namespace EngineG {

class TileMapComponent : public SpriteComponent {
public:
    TileMapComponent(Actor* owner, int drawOrder = 100);
    void Draw(SDL_Renderer* renderer) override;

    void LoadTileMapFromCSV(const std::vector<std::string>& csv);
    void SetTileMapRule();

private:
    std::vector<std::vector<int>> mTileRules;
    int mCellsHeight, mCellsWidth;
};

}  // namespace EngineG