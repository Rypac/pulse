#pragma once

#include "cocos2d.h"
#include "pulse/ui/Resources.hpp"

namespace pulse {

struct TextureAllocator {

    static void initialiseResources(cocos2d::TextureCache* textureCache = cocos2d::Director::getInstance()->getTextureCache()) {
        initialiseDigits(textureCache);
        initialiseButtons(textureCache);
    }

    static void initialiseDigits(cocos2d::TextureCache* textureCache) {
        for (auto&& digit : Resources::Images::Score::Digits) {
            textureCache->addImageAsync(digit, nullptr);
        }
    }

    static void initialiseButtons(cocos2d::TextureCache* textureCache) {
        textureCache->addImageAsync(Resources::Buttons::Achievements, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Blank, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Home, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Modes, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Pause, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Play, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Reset, nullptr);
        textureCache->addImageAsync(Resources::Buttons::Restart, nullptr);
    }
};

}
