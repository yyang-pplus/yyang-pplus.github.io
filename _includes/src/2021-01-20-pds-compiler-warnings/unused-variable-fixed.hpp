#include <cassert>

void UnusedVariableNoWarning() {
    [[maybe_unused]] const auto x = sizeof(char);
    assert(x);
}
