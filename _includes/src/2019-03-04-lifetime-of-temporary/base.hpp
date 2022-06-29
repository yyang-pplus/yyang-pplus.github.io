// base.hpp

#pragma once

#include "trace.hpp"

struct Base {
    Base() noexcept {
        TRACE_FUNCTION_CALL();
    }

    ~Base() noexcept {
        TRACE_FUNCTION_CALL();
    }
};
