// boost-stacktrace-on-assert.hpp

#pragma once

#include <cstdlib>
#include <iostream>

#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/stacktrace.hpp>

inline void
assertion_failed(const char *expr, const char *file, const long line, const char *function) {
    std::cerr << "Assertion '" << expr << "' failed at " << file << ':' << line << ": "
              << function << ".\n"
              << "Backtrace:\n"
              << boost::stacktrace::stacktrace() << std::endl;

    std::_Exit(EXIT_FAILURE);
}

#ifdef NDEBUG

#define AssertWithStacktrace(expr) void(0)

#else

#define AssertWithStacktrace(expr)       \
    (static_cast<bool>(expr) ? void(0) : \
                               assertion_failed(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))

#endif
