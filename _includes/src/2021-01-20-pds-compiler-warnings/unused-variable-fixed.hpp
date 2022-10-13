#include <cassert>

void UnusedVariableNoWarning() {
    const auto x = sizeof(char);
    assert(x);
    std::ignore = x;
}
