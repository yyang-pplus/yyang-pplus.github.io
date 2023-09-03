#include <cstdlib>

#include <exception>
#include <iostream>

[[noreturn]] void onTerminate() noexcept {
    if (const auto e = std::current_exception()) {
        std::cerr << "Caught unhandled exception." << std::endl;
        try {
            std::rethrow_exception(e);
        } catch (const std::exception &e) {
            // additional action
            std::cerr << e.what() << std::endl;
        } catch (...) {
            // additional action
            std::cerr << "Unknown exception." << std::endl;
        }
    }

    std::_Exit(EXIT_FAILURE);
}
