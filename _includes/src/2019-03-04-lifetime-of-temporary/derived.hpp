// derived.hpp

#pragma once

#include "base.hpp"

struct Derived : public Base {
    Derived() noexcept {
        TRACE_FUNCTION_CALL();
    }

    ~Derived() noexcept {
        TRACE_FUNCTION_CALL();
    }
};
