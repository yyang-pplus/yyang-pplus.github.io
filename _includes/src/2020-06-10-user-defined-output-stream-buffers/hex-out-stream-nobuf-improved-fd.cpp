// hex-out-stream-nobuf-improved-fd.cpp

#include "hex-out-stream-nobuf-improved.hpp"
#include "test-utils.hpp"

constexpr int STD_ERR_FD = 2;

int main() {
    HexOutBuf buffer {STD_ERR_FD};
    std::ostream out(&buffer);

    TestHelper(out);
}
