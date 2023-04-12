// test-utils.hpp

#pragma once

#include <iostream>

auto &TestHelper(std::istream &in) noexcept {
    for (int i = 0; true; ++i) {
        const auto peek_c = in.peek();
        const char get_c = in.get();

        if (not in) {
            break;
        }
        std::cout << get_c << '(' << peek_c << ')';

        if (i % 4 == 0) {
            in.unget();
        }
    }
    std::cout << std::endl;

    return in;
}
