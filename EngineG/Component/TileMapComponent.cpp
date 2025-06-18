#include "TileMapComponent.hpp"

#include "../Actor/Actor.hpp"
#include "../Math/Math.hpp"
using namespace EngineG::Math;

#include <iostream>

namespace EngineG {

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder) : SpriteComponent(owner, drawOrder) {
}

void TileMapComponent::Draw(SDL_Renderer* renderer) {
    if (mTexture && mTileRules.size() > 0) {
        SDL_Rect cell;
        cell.w = mTexWidth / 8;
        cell.h = cell.w;

        // int maxRows = mTexHeight / cell.h;
        int maxCols = mTexWidth / cell.w;

        SDL_Rect dst;
        dst.w = static_cast<int>(cell.w * mOwner->GetScale());
        dst.h = static_cast<int>(cell.h * mOwner->GetScale());

        Vector2 r;
        // Center the rectangle around the position of the owner
        r.x = static_cast<int>(mOwner->GetPosition().x - (dst.w * mTileRules[0].size()) / 2);
        r.y = static_cast<int>(mOwner->GetPosition().y - (dst.h * mTileRules.size()) / 2);

        int rowIndex = 0;
        for (const auto& rowTiles : mTileRules) {
            int colIndex = 0;
            for (const auto tile : rowTiles) {
                if (tile == -1) {
                    ++colIndex;
                    continue;
                }

                // Fill Cell with texture
                int texCol = (tile) % maxCols;
                int texRow = (tile) / maxCols;

                cell.x = cell.w * texCol;
                cell.y = cell.h * texRow;

                // Set distination of cell to draw onto the target
                dst.x = static_cast<int>(r.x + (colIndex * dst.w));
                dst.y = static_cast<int>(r.y + (rowIndex * dst.h));

                // Draw (have to convert angle from radians to degrees, and clockwise to counter)
                SDL_RenderCopyEx(renderer,
                                 mTexture,
                                 &cell,                                    // Part of texture to draw (null if whole)
                                 &dst,                                     // Rectangle to draw onto the target
                                 -Math::ToDegrees(mOwner->GetRotation()),  // Rotation angle (in degrees, clockwise)
                                 nullptr,                                  // Point to rotate about (nullptr for center)
                                 isReversed ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
                ++colIndex;
            }
            ++rowIndex;
        }
    }
}

void TileMapComponent::SetTileMapRule() {
}

void TileMapComponent::LoadTileMapFromCSV(const std::vector<std::string>& csv) {
    for (const auto& row : csv) {
        std::string row_str = row;
        std::vector<int> row_int;

        while (row_str.find(",") != std::string::npos) {
            std::string num_str = row_str.substr(0, row_str.find(","));
            int next_num = stoi(num_str);
            row_str = row_str.substr(row_str.find(",") + 1, row_str.length());
            row_int.push_back(next_num);
        }
        std::string num_str = row_str.substr(0, row_str.length());
        int next_num = stoi(num_str);
        row_int.push_back(next_num);

        mTileRules.push_back(row_int);
    }
}

}  // namespace EngineG
