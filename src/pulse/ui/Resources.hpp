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
        static constexpr const auto Achievements = "buttons/achievements.png";
        static constexpr const auto Blank = "buttons/blank.png";
        static constexpr const auto Home = "buttons/home.png";
        static constexpr const auto Modes = "buttons/modes.png";
        static constexpr const auto Pause = "buttons/pause.png";
        static constexpr const auto Play = "buttons/play.png";
        static constexpr const auto Resume = "buttons/resume.png";
        static constexpr const auto Reset = "buttons/reset.png";
        static constexpr const auto Restart = "buttons/restart.png";
        static constexpr const auto Settings = "buttons/settings.png";
        static constexpr const auto Tick = "buttons/tick.png";
    }

    namespace Images {
        namespace Intro {
            static constexpr const auto Short = "tandem_intro_short.png";
            static constexpr const auto Long = "tandem_intro_long.png";
        }

        namespace Title {
            static constexpr const auto Logo = "title/logo.png";
        }

        namespace Score {
            static constexpr const auto Banner = "score/banner.png";
            static constexpr const auto Zero = "score/0.png";
            static constexpr const auto One = "score/1.png";
            static constexpr const auto Two = "score/2.png";
            static constexpr const auto Three = "score/3.png";
            static constexpr const auto Four = "score/4.png";
            static constexpr const auto Five = "score/5.png";
            static constexpr const auto Six = "score/6.png";
            static constexpr const auto Seven = "score/7.png";
            static constexpr const auto Eight = "score/8.png";
            static constexpr const auto Nine = "score/9.png";
            static constexpr const char* Digits[] = {
                Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine
            };
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
        static constexpr const auto AmbientBackground = "particles/background_sparkles.plist";
        static constexpr const auto ButtonBackground = "particles/button_sparkles.plist";
        static constexpr const auto PlayerTrail = "particles/player_trail.plist";
        static constexpr const auto PulseBegan = "particles/pulse_began.plist";
        static constexpr const auto PulseEnded = "particles/pulse_ended.plist";
    }

    namespace Spritesheets {
        static constexpr const auto Intro = "animations/splash/tandem_intro_spritesheet.plist";
    }
}

}
