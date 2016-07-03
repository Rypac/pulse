#pragma once

namespace pulse {

namespace Resources {

    namespace Animations {
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
        static constexpr const auto Achievements = "achievements.png";
        static constexpr const auto Blank = "blank.png";
        static constexpr const auto Home = "home.png";
        static constexpr const auto Modes = "modes.png";
        static constexpr const auto Pause = "pause.png";
        static constexpr const auto Play = "play.png";
        static constexpr const auto Resume = "resume.png";
        static constexpr const auto Reset = "reset.png";
        static constexpr const auto Restart = "restart.png";
        static constexpr const auto Settings = "settings.png";
        static constexpr const auto Tick = "tick.png";
    }

    namespace Images {
        namespace Intro {
            static constexpr const auto Short = "tandem_intro_short.png";
            static constexpr const auto Long = "tandem_intro_long.png";
        }

        static constexpr const auto Banner = "banner.png";
        static constexpr const auto Logo = "letters.png";
        static constexpr const char* Digits[] = {
            "0.png", "1.png", "2.png", "3.png", "4.png", "5.png", "6.png", "7.png", "8.png", "9.png"
        };

        namespace Menu {
            namespace Slider {
                static constexpr const auto Disabled = "slider/disabled.png";
                static constexpr const auto Empty = "slider/empty.png";
                static constexpr const auto Filled = "slider/filled.png";
                static constexpr const auto Normal = "slider/normal.png";
                static constexpr const auto Selected = "slider/selected.png";
            }
        }
    }

    namespace Particles {
        static constexpr const auto AmbientBackground = "particles/background_sparkles.plist";
        static constexpr const auto ButtonBackground = "particles/button_sparkles.plist";
        static constexpr const auto PlayerTrail = "particles/player_trail.plist";
        static constexpr const auto PulseBegan = "particles/pulse_began.plist";
        static constexpr const auto PulseEnded = "particles/pulse_ended.plist";
    }

    namespace Spritesheets {
        static constexpr const auto Banner = "banner.plist";
        static constexpr const auto Buttons = "buttons.plist";
        static constexpr const auto Digits = "digits.plist";
        static constexpr const auto Intro = "animations/splash/tandem_intro_spritesheet.plist";
        static constexpr const auto Letters = "letters.plist";
        static constexpr const auto Menu = "menu.plist";
    }
}

}
