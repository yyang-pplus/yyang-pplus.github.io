#pragma once

#include <iostream>

// clang-format off
#define TRACE_FUNCTION_CALL() {                     \
    std::cout << __PRETTY_FUNCTION__ << std::endl;  \
}
// clang-format on
