// exception-4.cpp

#include "base-wrapper.hpp"

// clang-format off
int main() {
    auto *w = new BaseWrapper { {} };
    TRACE_FUNCTION_CALL();
    delete w;
}
// clang-format on
