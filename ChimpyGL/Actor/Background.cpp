#include "Background.hpp"

#include "Component/Game.hpp"
#include "Component/TileMapComponent.hpp"

namespace EngineG {

Background::Background(class Game* game) : Actor(game), mLayers() {
    auto* texture = game->GetTexture("Tiles.png");

    auto* layer3 = new TileMapComponent(this, 30);
    layer3->SetTexture(texture);
    layer3->LoadTileMapFromCSV(game->GetCSV("MapLayer3.csv"));

    auto* layer2 = new TileMapComponent(this, 31);
    layer2->SetTexture(texture);
    layer2->LoadTileMapFromCSV(game->GetCSV("MapLayer2.csv"));

    auto* layer1 = new TileMapComponent(this, 32);
    layer1->SetTexture(texture);
    layer1->LoadTileMapFromCSV(game->GetCSV("MapLayer1.csv"));

    mLayers.push_back(layer3);
    mLayers.push_back(layer2);
    mLayers.push_back(layer1);
}

}  // namespace EngineG
