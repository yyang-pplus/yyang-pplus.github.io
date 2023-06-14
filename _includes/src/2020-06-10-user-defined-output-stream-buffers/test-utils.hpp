// test-utils.hpp

#pragma once

#include <ostream>

inline auto &TestHelper(std::ostream &out) {
    out << "1234" << '\0';
    out << "IJK" << '\0';
    out << "ab" << '\0';
    return out << '\n' << '\0';
}
