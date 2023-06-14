#include "sliding-log-logger.hpp"
#include "test-utils.hpp"

int main() {
    TestLimiterLogger(SlidingLogLogger {3});
}
