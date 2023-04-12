// rvalue-reference.cpp

#include "base.hpp"

int main() {
    auto &&b = Base {};
    TRACE_FUNCTION_CALL();
}
