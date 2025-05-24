#include "ScreenManager.hpp"

#include "Component/Game.hpp"

namespace EngineG {

ScreenManager::ScreenManager(class Game* game) : Actor(game), mScreens() {
}

void ScreenManager::ChangeScreen(ScreenState newState) {
    for (auto it = mScreens.begin(); it != mScreens.end(); ++it) {
        if ((*it)->GetType() == newState) {
            mCurScreen = *it;
            mCurState = newState;
            return;
        }
    }

    throw std::runtime_error("ScreenManager::ChangeScreen: Screen not found");
}

void ScreenManager::AddScreen(ScreenState state, class ScreenComponent<ScreenState>* screen) {
    mScreens.push_back(screen);
}

}  // namespace EngineG