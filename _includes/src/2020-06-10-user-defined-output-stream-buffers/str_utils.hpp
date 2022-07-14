// str_utils.hpp

#pragma once

#include <iomanip>
#include <sstream>

[[nodiscard]] inline auto ToHex(const unsigned c, const int width) noexcept {
    std::ostringstream oss;
    oss << std::setw(width) << std::setfill('0') << std::hex << c;
    return oss.str();
}
