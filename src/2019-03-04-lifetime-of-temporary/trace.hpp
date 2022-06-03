#pragma once

#include <iostream>

// clang-format off

#define TRACE_FUNCTION_CALL() {                                     \
    std::cout << "Inside:  " << __PRETTY_FUNCTION__ << std::endl;   \
}

#define TRACED_SCOPE(name)                                          \
    for(auto flag = true;                                           \
        flag and std::cout << "Scope Begin (" << name << ") {\n";   \
        flag = false,                                               \
        std::cout << "} End Scope\n\n" << std::endl)

// clang-format on
