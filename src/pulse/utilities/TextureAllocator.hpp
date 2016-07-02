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
            textureCache->addImage(digit);
        }
    }

    static void initialiseButtons(cocos2d::TextureCache* textureCache) {
        textureCache->addImage(Resources::Buttons::Achievements);
        textureCache->addImage(Resources::Buttons::Blank);
        textureCache->addImage(Resources::Buttons::Home);
        textureCache->addImage(Resources::Buttons::Modes);
        textureCache->addImage(Resources::Buttons::Pause);
        textureCache->addImage(Resources::Buttons::Play);
        textureCache->addImage(Resources::Buttons::Reset);
        textureCache->addImage(Resources::Buttons::Restart);
    }
};

}
