#pragma once

#include <type_traits>

template <typename T, typename... Args>
inline void safe_callback(T callback, Args&&... args) {
    if (callback) {
        callback(std::forward<Args>(args)...);
    }
}
