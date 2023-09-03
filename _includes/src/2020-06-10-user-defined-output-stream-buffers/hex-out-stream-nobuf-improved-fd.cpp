// hex-out-stream-nobuf-improved-fd.cpp

#include "hex-out-stream-nobuf-improved.hpp"
#include "test-utils.hpp"


int main() {
    HexOutBuf buffer {STDERR_FILENO};
    std::ostream out(&buffer);

    TestHelper(out);
}
