// exception-1.cpp

#include "base.hpp"

const Base &FunctionException1() {
    TRACE_FUNCTION_CALL();
    return Base {};
}

int main() {
    const auto &b = FunctionException1();
    TRACE_FUNCTION_CALL();
}
