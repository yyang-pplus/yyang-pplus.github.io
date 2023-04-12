auto f(const int x) {
    if (x < -1) {
        return x + 1;
    } else if (x >= 1) {
        return x - 1;
    } else {
        assert(x == -1 or x == 0);
        return x;
    }
}
