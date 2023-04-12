// hex-in-stream-buffer.cpp

#include "hex-in-stream-buffer.hpp"
#include "test-utils.hpp"

int main() {
    HexInBuf buffer;
    std::istream in(&buffer);

    TestHelper(in);
}
