/// @module ftxui.component:Animation
/// @brief Component 模块中 Animation 命名空间的 C++20 模块接口。
///

module;

#include <ftxui/component/animation.hpp>

export module ftxui.component:Animation;

/**
 * @namespace ftxui::animation
 * @brief FTXUI ftxui::animation:: 命名空间
 */
export namespace ftxui::animation {
    using ftxui::animation::RequestAnimationFrame;

    using ftxui::animation::Clock;
    using ftxui::animation::TimePoint;
    using ftxui::animation::Duration;

    using ftxui::animation::Params;

    /**
     * @namespace easing
     * @brief FTXUI ftxui::animation::easing:: 命名空间
     */
    namespace easing {
        using ftxui::animation::easing::Function;

        using ftxui::animation::easing::Linear;
        using ftxui::animation::easing::QuadraticIn;
        using ftxui::animation::easing::QuadraticOut;
        using ftxui::animation::easing::QuadraticInOut;
        using ftxui::animation::easing::CubicIn;
        using ftxui::animation::easing::CubicOut;
        using ftxui::animation::easing::CubicInOut;
        using ftxui::animation::easing::QuarticIn;
        using ftxui::animation::easing::QuarticOut;
        using ftxui::animation::easing::QuarticInOut;
        using ftxui::animation::easing::QuinticIn;
        using ftxui::animation::easing::QuinticOut;
        using ftxui::animation::easing::QuinticInOut;
        using ftxui::animation::easing::SineIn;
        using ftxui::animation::easing::SineOut;
        using ftxui::animation::easing::SineInOut;
        using ftxui::animation::easing::CircularIn;
        using ftxui::animation::easing::CircularOut;
        using ftxui::animation::easing::CircularInOut;
        using ftxui::animation::easing::ExponentialIn;
        using ftxui::animation::easing::ExponentialOut;
        using ftxui::animation::easing::ExponentialInOut;
        using ftxui::animation::easing::ElasticIn;
        using ftxui::animation::easing::ElasticOut;
        using ftxui::animation::easing::ElasticInOut;
        using ftxui::animation::easing::BackIn;
        using ftxui::animation::easing::BackOut;
        using ftxui::animation::easing::BackInOut;
        using ftxui::animation::easing::BounceIn;
        using ftxui::animation::easing::BounceOut;
        using ftxui::animation::easing::BounceInOut;
    }

    using ftxui::animation::Animator;
}
