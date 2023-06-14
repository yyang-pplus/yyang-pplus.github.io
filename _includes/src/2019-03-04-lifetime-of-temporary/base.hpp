// base.hpp

#pragma once

#include "trace.hpp"

struct Base {
    Base() {
        TRACE_FUNCTION_CALL();
    }

    ~Base() {
        TRACE_FUNCTION_CALL();
    }
};
