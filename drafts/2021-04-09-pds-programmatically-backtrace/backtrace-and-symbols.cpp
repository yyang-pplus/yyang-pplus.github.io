#include "backtrace-and-symbols.hpp"

#include "dummy-functions.hpp"


int main() {
    dummy_function(1, write_backtrace_to_stderr);
}
