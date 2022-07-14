// hex_out_stream_nobuf_improved_fd.cpp

#include "hex_out_stream_nobuf_improved.hpp"
#include "test_utils.hpp"

constexpr int STD_ERR_FD = 2;

int main() {
    HexOutBuf buffer {STD_ERR_FD};
    std::ostream out(&buffer);

    TestHelper(out);
}
