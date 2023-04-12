#include <cstdlib>

auto TriangleArea(const double height, const double base) {
    if (height <= 0 or base <= 0) {
        std::abort();
    }

    return height * base / 2.0;
}
