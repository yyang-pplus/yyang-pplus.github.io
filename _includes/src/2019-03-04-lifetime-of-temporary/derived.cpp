// derived.cpp

#include "derived.hpp"

int main() {
    const Base &b = Derived {};
    TRACE_FUNCTION_CALL();
}
