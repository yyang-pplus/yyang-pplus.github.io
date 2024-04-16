// dummy-functions.hpp

#pragma once

#include <functional>

inline void dummy_function(double, const std::function<void(void)> f) {
    f();
}

inline void dummy_function(int, const std::function<void(void)> f) {
    dummy_function(1.0, f);
}
