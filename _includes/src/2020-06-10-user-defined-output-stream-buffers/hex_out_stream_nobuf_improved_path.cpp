// hex_out_stream_nobuf_improved_path.cpp

#include "hex_out_stream_nobuf_improved.hpp"
#include "test_utils.hpp"

int main() {
    const char *pathname = "/tmp/hex-out-test-file.txt";

    HexOutBuf buffer {
        pathname, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH};

    if (buffer.IsOpen()) {
        std::ostream out(&buffer);
        TestHelper(out);
    }
}
