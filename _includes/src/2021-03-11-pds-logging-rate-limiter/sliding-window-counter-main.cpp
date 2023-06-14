#include "sliding-window-counter-logger.hpp"
#include "test-utils.hpp"

int main() {
    TestLimiterLogger(SlidingWindowCounterLogger {3});
}
