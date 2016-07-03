#pragma once

#include "cocos2d.h"
#include "pulse/ui/Resources.hpp"

namespace pulse {

struct TextureAllocator {

    static void initialiseResources() {
        initialiseSpritesheet(cocos2d::SpriteFrameCache::getInstance());
    }

    static void initialiseSpritesheet(cocos2d::SpriteFrameCache* spriteCache) {
        spriteCache->addSpriteFramesWithFile(Resources::Spritesheets::Banner);
        spriteCache->addSpriteFramesWithFile(Resources::Spritesheets::Buttons);
        spriteCache->addSpriteFramesWithFile(Resources::Spritesheets::Digits);
        spriteCache->addSpriteFramesWithFile(Resources::Spritesheets::Intro);
        spriteCache->addSpriteFramesWithFile(Resources::Spritesheets::Letters);
        spriteCache->addSpriteFramesWithFile(Resources::Spritesheets::Menu);
    }
};

}
