#include <iostream>
#include <mutex>

#define BOOST_STACKTRACE_LINK
#include <boost/stacktrace.hpp>

#include "dummy-functions.hpp"


auto &get_terminate_mutex() {
    static std::mutex m;
    return m;
}

[[noreturn]] void on_terminate() noexcept {
    std::unique_lock lock(get_terminate_mutex(), std::defer_lock);
    try {
        lock.lock();
        std::cerr << boost::stacktrace::stacktrace() << std::endl;
    } catch (...) {
    }

    std::_Exit(EXIT_FAILURE);
}

const auto installed = []() {
    get_terminate_mutex();
    return std::set_terminate(&on_terminate);
}();

inline void just_throw() {
    throw std::out_of_range {"Hello Exception"};
}

int main() {
    dummy_function(1, just_throw);
}
