#include "catch-current-exception-on-terminate.hpp"


const auto installed {std::set_terminate(&onTerminate)};

int main() {
    throw std::out_of_range {"Hello Exception"};
}
