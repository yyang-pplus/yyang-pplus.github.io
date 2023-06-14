// chrono-utils.hpp

#pragma once

#include <chrono>
#include <iomanip>
#include <ostream>

static inline auto &operator<<(std::ostream &out,
                               const std::chrono::system_clock::time_point &tp) noexcept {
    constexpr auto *format = "%F %T %Z";

    const auto tt = std::chrono::system_clock::to_time_t(tp);
    const auto *tm = std::gmtime(&tt); //not thread-safe
    if (tm) {
        out << std::put_time(tm, format);
    }

    return out;
}
