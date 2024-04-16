// backtrace-and-symbols.hpp

#pragma once

#include <execinfo.h>
#include <unistd.h>


inline void write_backtrace(const int fd) {
    constexpr auto CAPACITY = 50;
    void *array[CAPACITY] = {};
    constexpr auto SKIP = 1;

    const auto size = backtrace(array, CAPACITY);
    backtrace_symbols_fd(array + SKIP, size, fd);
}

inline void write_backtrace_to_stderr() {
    write_backtrace(STDERR_FILENO);
}
