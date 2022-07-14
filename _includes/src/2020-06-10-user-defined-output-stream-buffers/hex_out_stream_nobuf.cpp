// hex_out_stream_nobuf.cpp

#include "hex_out_stream_nobuf.hpp"
#include "test_utils.hpp"

int main() {
    HexOutBuf buffer;
    std::ostream out(&buffer);

    TestHelper(out);
}
