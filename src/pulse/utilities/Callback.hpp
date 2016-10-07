#pragma once

#include <type_traits>

template <typename Fn, typename... Args>
inline void safe_callback(Fn callback, Args&&... args) {
    if (callback) {
        callback(std::forward<Args>(args)...);
    }
}

template <typename T, typename Fn>
inline auto member_callback(T* obj, Fn func) {
    return [obj, func](auto&&... args) {
        return (obj->*func)(std::forward<decltype(args)>(args)...);
    };
}

template <typename T, typename Fn>
inline auto member_callback(T&& obj, Fn func) {
    return [ obj = std::forward<T>(obj), func ](auto&&... args) mutable {
        return (obj.*func)(std::forward<decltype(args)>(args)...);
    };
}
