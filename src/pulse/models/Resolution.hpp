#pragma once

#include <string>
#include <vector>
#include "cocos2d.h"

namespace pulse {

struct Resolution {
    const cocos2d::Size size;
    const std::string path;

    static const Resolution& forFrame(const cocos2d::Size& frame);
    static const Resolution& design();

    float contentScaleFactor() const;
    std::vector<std::string> resourceSearchPaths() const;
};

}
