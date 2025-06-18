#include "AnimSpriteComponent.hpp"

#include "../Math/Math.hpp"
using namespace EngineG::Math;
#include <ostream>

namespace EngineG {

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
    : SpriteComponent(owner, drawOrder), mState(ESingle), mCurrFrame(0.0f), mAnimFPS(24.0f), mCurrAnim(0) {
}

void AnimSpriteComponent::Update(float deltaTime) {
    SpriteComponent::Update(deltaTime);

    if (mAnimTextures.size() > 0) {
        // Update the current frame based on frame rate
        // and delta time
        mCurrFrame += mAnimFPS * deltaTime;

        // Wrap current frame if needed
        if (mState == ESingle) {
            while (mCurrFrame >= mAnimTextures.size()) {
                mCurrFrame -= mAnimTextures.size();
            }
            // Set the current texture
            SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
        }

        // Multiple animations
        else if (mState == EMultiple) {
            while (mCurrFrame >= mAnimRanges[mCurrAnim].size) {
                if (!mAnimRanges[mCurrAnim].mLoop) {
                    mCurrFrame = mAnimRanges[mCurrAnim].size - 1;
                    break;
                }
                mCurrFrame -= mAnimRanges[mCurrAnim].size;
            }
            // Set the current texture
            SetTexture(mAnimTextures[static_cast<int>(mCurrFrame + mAnimRanges[mCurrAnim].mStart)]);
        }

        else {
            std::__throw_logic_error("Invalid state");
        }
    }
}

void AnimSpriteComponent::SetAnimRanges(const std::vector<Animation>& ranges) {
    mAnimRanges = ranges;
    if (mAnimRanges.size() > 0) {
        for (auto& animation : mAnimRanges) {
            --animation.mEnd;
            --animation.mStart;
        }
        mState = EMultiple;
        mCurrAnim = 0;
    }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures) {
    mAnimTextures = textures;
    if (mAnimTextures.size() > 0) {
        // Set the active texture to first frame
        mCurrFrame = 0.0f;
        SetTexture(mAnimTextures[0]);
    }
}

void AnimSpriteComponent::SetNumAnim(int nextAnimation) {
    mCurrAnim = nextAnimation;
    if (mCurrAnim >= mAnimTextures.size()) {
        std::__throw_logic_error("Invalid number of animations");
    }
}

}  // namespace EngineG
