// test-utils.hpp

#pragma once

template<typename Logger>
void TestLimiterLogger(Logger &&logger) {
    while (true) {
        logger.Info("");
    }
}
