// hex-out-stream-nobuf-improved-path.cpp

#include "hex-out-stream-nobuf-improved.hpp"
#include "test-utils.hpp"

int main() {
    const char *pathname = "/tmp/hex-out-test-file.txt";

    HexOutBuf buffer {
        pathname, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH};

    if (buffer.IsOpen()) {
        std::ostream out(&buffer);
        TestHelper(out);
    }
}
