// test-utils.hpp

#pragma once

#include <cstdlib>


template<typename Logger>
void TestLimiterLogger(Logger &&logger) {
    const auto *limited = std::getenv("LIMITED");
    for (int i = 10; i > 0 or not limited; --i) {
        logger.Info("");
    }
}
