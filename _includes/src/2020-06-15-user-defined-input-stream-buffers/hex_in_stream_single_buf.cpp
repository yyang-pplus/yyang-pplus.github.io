// hex_in_stream_single_buf.cpp

#include "hex_in_stream_single_buf.hpp"
#include "test_utils.hpp"

int main() {
    HexInBuf buffer;
    std::istream in(&buffer);

    TestHelper(in);
}
