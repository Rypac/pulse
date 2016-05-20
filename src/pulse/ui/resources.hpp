#pragma once

namespace pulse {

namespace Resources {

    namespace Animations {
        static constexpr const auto Title = "animations/title.c3b";

        namespace Intro {
            static constexpr const auto Properties = "animations/splash/tandem_intro.plist";
            static constexpr const auto Short = "tandem_intro_short";
            static constexpr const auto Long = "tandem_intro_long";
        }
    }

    namespace Audio {
        namespace Intro {
            static constexpr const auto Short = "audio/splash/tandem_intro_short.wav";
            static constexpr const auto Long = "audio/splash/tandem_intro_long.wav";
        }
    }

    namespace Buttons {
        static constexpr const auto Play = "buttons/play.png";
        static constexpr const auto Mode = "buttons/mode.png";
        static constexpr const auto Achievments = "buttons/achievments.png";
        static constexpr const auto Settings = "buttons/settings.png";
    }

    namespace Images {
        namespace Intro {
            static constexpr const auto Short = "tandem_intro_short.png";
            static constexpr const auto Long = "tandem_intro_long.png";
        }

        namespace Menu {
            namespace Slider {
                static constexpr const auto Empty = "menu/slider/empty.png";
                static constexpr const auto Filled = "menu/slider/filled.png";
                static constexpr const auto Normal = "menu/slider/ball/normal.png";
                static constexpr const auto Selected = "menu/slider/ball/selected.png";
                static constexpr const auto Disabled = "menu/slider/ball/disabled.png";
            }
        }
    }

    namespace Particles {
        static constexpr const auto AmbientBackground = "particles/ambient_sparkles.plist";
        static constexpr const auto PulseBegan = "particles/pulse_began.plist";
        static constexpr const auto PulseEnded = "particles/pulse_ended.plist";
    }

    namespace Spritesheets {
        static constexpr const auto Intro = "animations/splash/tandem_intro_spritesheet.plist";
    }
}

}
