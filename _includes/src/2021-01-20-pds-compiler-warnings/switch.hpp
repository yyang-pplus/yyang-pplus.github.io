enum class MyEnum {
    one,
    two,
    three,
};

void Dispatch(const MyEnum e) {
    switch (e) {
    case MyEnum::one:
        break;
    case MyEnum::two:
        break;
    }
}
