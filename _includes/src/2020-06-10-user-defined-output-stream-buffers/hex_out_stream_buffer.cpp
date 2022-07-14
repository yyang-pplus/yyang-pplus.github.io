// hex_out_stream_buffer.cpp

#include "hex_out_stream_buffer.hpp"
#include "test_utils.hpp"

int main() {
    HexOutBuf buffer;
    std::ostream out(&buffer);

    TestHelper(out);
}
