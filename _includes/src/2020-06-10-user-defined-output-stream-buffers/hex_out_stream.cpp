// hex_out_stream.cpp

#include "hex_out_stream.hpp"
#include "test_utils.hpp"

constexpr int STD_ERR_FD = 2;

int main() {
    HexOStream out {STD_ERR_FD};

    TestHelper(out);
}
