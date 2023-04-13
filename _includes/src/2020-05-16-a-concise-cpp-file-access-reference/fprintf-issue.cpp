#include <cstdio>

int main() {
    auto fp = std::fopen("/dev/null", "w");

    if (fp) {
#include "fprintf-issue.hpp"
        std::fclose(fp);
    }
}
