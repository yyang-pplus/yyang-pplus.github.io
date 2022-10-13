template<typename T>
auto Pad(const T v, [[maybe_unused]] const int extra = sizeof(int)) {
    if constexpr (sizeof(T) < sizeof(long)) {
        return v + extra;
    }
    return v;
}
