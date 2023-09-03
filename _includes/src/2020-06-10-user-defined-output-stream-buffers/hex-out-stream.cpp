// hex-out-stream.cpp

#include "hex-out-stream.hpp"
#include "test-utils.hpp"


int main() {
    HexOStream out {STDERR_FILENO};

    TestHelper(out);
}
