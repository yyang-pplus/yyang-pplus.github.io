// hex-in-stream-nobuf.cpp

#include "hex-in-stream-nobuf.hpp"
#include "test-utils.hpp"

int main() {
    HexInBuf buffer;
    std::istream in(&buffer);

    TestHelper(in);
}
