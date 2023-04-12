// hex-in-stream-single-buf.cpp

#include "hex-in-stream-single-buf.hpp"
#include "test-utils.hpp"

int main() {
    HexInBuf buffer;
    std::istream in(&buffer);

    TestHelper(in);
}
