// hex-out-stream-buffer.cpp

#include "hex-out-stream-buffer.hpp"
#include "test-utils.hpp"

int main() {
    HexOutBuf buffer;
    std::ostream out(&buffer);

    TestHelper(out);
}
