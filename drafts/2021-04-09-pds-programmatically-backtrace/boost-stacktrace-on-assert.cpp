#include "boost-stacktrace-on-assert.hpp"
#include "dummy-functions.hpp"

inline void just_assert() {
    AssertWithStacktrace(false);
}

int main() {
    dummy_function(1, just_assert);
}
