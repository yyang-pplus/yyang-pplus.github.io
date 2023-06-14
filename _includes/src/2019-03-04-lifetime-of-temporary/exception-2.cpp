// exception-2.cpp

#include "derived.hpp"

struct DerivedWrapper {
    DerivedWrapper() : d {} {
        TRACE_FUNCTION_CALL();
    }

    ~DerivedWrapper() {
        TRACE_FUNCTION_CALL();
    }

    const Derived &d;
};

int main() {
    DerivedWrapper dw;
    TRACE_FUNCTION_CALL();
}
