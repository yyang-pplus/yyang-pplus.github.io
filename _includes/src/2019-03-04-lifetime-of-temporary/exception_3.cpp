// exception_3.cpp

#include "base.hpp"

const Base &FunctionException3(const Base &b) noexcept {
    TRACE_FUNCTION_CALL();
    return b;
}

int main() {
    const auto &b = FunctionException3({});
    TRACE_FUNCTION_CALL();
}
