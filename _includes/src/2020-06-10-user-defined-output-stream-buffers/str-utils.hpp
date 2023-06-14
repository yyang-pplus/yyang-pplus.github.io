// str-utils.hpp

#pragma once

#include <iomanip>
#include <sstream>

inline auto ToHex(const unsigned c, const int width) {
    std::ostringstream oss;
    oss << std::setw(width) << std::setfill('0') << std::hex << c;
    return oss.str();
}
