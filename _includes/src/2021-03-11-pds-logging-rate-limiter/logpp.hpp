// logpp.hpp

#pragma once

#include <iostream>
#include <iterator>
#include <string_view>

#include "chrono-utils.hpp"

class Logpp {
public:
    enum class Level {
        debug = 'D',
        info = 'I',
        warning = 'W',
        error = 'E',
        critical = 'C',
    };

    auto Log(const Level a_level, const std::string_view message) const {
        const auto now = std::chrono::system_clock::now();
        std::cout << '[' << now << ']' << '(' << static_cast<char>(a_level) << ") " << message
                  << std::endl;
        return true;
    }
};
