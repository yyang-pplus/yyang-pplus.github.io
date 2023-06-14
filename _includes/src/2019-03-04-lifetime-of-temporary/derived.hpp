// derived.hpp

#pragma once

#include "base.hpp"

struct Derived : public Base {
    Derived() {
        TRACE_FUNCTION_CALL();
    }

    ~Derived() {
        TRACE_FUNCTION_CALL();
    }
};
