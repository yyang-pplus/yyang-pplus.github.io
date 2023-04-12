#include <cassert>

#include "assert-before-crash.hpp"


int main() {
    int n = 2;
    Half(&n);

    Half(nullptr);
}
