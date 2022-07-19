// hex_in_stream_buffer.cpp

#include "hex_in_stream_buffer.hpp"
#include "test_utils.hpp"

int main() {
    HexInBuf buffer;
    std::istream in(&buffer);

    TestHelper(in);
}
