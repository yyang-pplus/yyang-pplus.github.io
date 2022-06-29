// const_lvalue_reference.cpp

#include "base.hpp"

int main() {
    const auto &b = Base {};
    TRACE_FUNCTION_CALL();
}
