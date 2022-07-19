// hex_in_stream_nobuf.cpp

#include "hex_in_stream_nobuf.hpp"
#include "test_utils.hpp"

int main() {
    HexInBuf buffer;
    std::istream in(&buffer);

    TestHelper(in);
}
