#include "derived.hpp"

struct DerivedWrapper {
    DerivedWrapper() noexcept : d {} {
        TRACE_FUNCTION_CALL();
    }

    ~DerivedWrapper() noexcept {
        TRACE_FUNCTION_CALL();
    }

    const Derived &d;
};

int main() {
    DerivedWrapper dw;
    TRACE_FUNCTION_CALL();
}
