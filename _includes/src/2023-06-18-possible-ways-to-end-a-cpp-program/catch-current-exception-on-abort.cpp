#include "catch-current-exception-on-terminate.hpp"

#include <csignal>


void signal_handler(const int) {
    onTerminate();
}

int main() {
    // Don't do this on production, as onTerminate() is not signal-safe.
    std::signal(SIGABRT, signal_handler);

    throw std::out_of_range {"Hello Exception"};
}
