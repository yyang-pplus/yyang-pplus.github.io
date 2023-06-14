#include "test-utils.hpp"
#include "token-bucket-logger.hpp"

int main() {
    TestLimiterLogger(TokenBucketLogger {3});
}
