// hex-out-stream.cpp

#include "hex-out-stream.hpp"
#include "test-utils.hpp"

constexpr int STD_ERR_FD = 2;

int main() {
    HexOStream out {STD_ERR_FD};

    TestHelper(out);
}
