#include <cassert>

int main() {
    assert(true && "Good assumption");
    assert(("Bad assumption", false));
}
