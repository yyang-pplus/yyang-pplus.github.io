#include "leaky-bucket-logger.hpp"
#include "test-utils.hpp"

int main() {
    LeakyBucketLogger logger {3};
    logger.Start();
    TestLimiterLogger(logger);
}
