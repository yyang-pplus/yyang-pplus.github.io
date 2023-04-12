// hex-out-stream-nobuf.cpp

#include "hex-out-stream-nobuf.hpp"
#include "test-utils.hpp"

int main() {
    HexOutBuf buffer;
    std::ostream out(&buffer);

    TestHelper(out);
}
