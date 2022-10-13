auto FallThroughNoWarning(const int n) {
    int result = 0;

    switch (n) {
    case 1:
        result = 1;
        [[fallthrough]];
    default:
        ++result;
    }

    return result;
}
