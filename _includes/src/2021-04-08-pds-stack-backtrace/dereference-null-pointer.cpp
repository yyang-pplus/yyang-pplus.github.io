void half(int *const pointer) {
    *pointer /= 2;
}

auto load_config() {
    return 2;
}

void stepOne() {
    int one {};
    half(&one);
}

void stepTwo() {
    int two[2] {};
    half(two);
}

void stepThree() {
    int *three {};
    half(three);
}

void process(const int config) {
    stepOne();

    if (config) {
        stepTwo();
    }

    stepThree();
}

int main() {
    const auto config = load_config();
    process(config);
}
