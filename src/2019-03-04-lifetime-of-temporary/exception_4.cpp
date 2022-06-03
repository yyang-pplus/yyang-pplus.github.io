#include "base_wrapper.hpp"

int main() {
    auto *w = new BaseWrapper {{}};
    TRACE_FUNCTION_CALL();
    delete w;
}
