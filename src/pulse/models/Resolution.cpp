#include "pulse/models/Resolution.hpp"

using pulse::Resolution;
using cocos2d::Size;

namespace SupportedResolutions {
    static const auto Small = Resolution{Size{960, 540}, "small"};
    static const auto Medium = Resolution{Size{1280, 720}, "medium"};
    static const auto Large = Resolution{Size{1920, 1080}, "large"};
    static const auto ExtraLarge = Resolution{Size{2560, 1440}, "extra_large"};
}

const Resolution& Resolution::design() {
    return SupportedResolutions::Medium;
}

const Resolution& Resolution::forFrame(const Size& frame) {
    if (frame.height > SupportedResolutions::Large.size.height) {
        return SupportedResolutions::ExtraLarge;
    } else if (frame.height > SupportedResolutions::Medium.size.height) {
        return SupportedResolutions::Large;
    } else if (frame.height > SupportedResolutions::Small.size.height) {
        return SupportedResolutions::Medium;
    } else {
        return SupportedResolutions::Small;
    }
}

float Resolution::contentScaleFactor() const {
    const auto designResolution = design();
    const auto scaledHeight = size.height / designResolution.size.height;
    const auto scaledWidth = size.width / designResolution.size.width;
    return std::min(scaledHeight, scaledWidth);
}

std::vector<std::string> Resolution::resourceSearchPaths() const {
    return {"images/" + path};
}
