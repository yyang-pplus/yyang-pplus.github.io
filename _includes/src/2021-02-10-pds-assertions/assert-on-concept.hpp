#include <concepts>


struct MyStruct {
    MyStruct() = default;
    MyStruct(const MyStruct &) = delete;
};


//static_assert(std::copy_constructible<MyStruct>);  // Error
static_assert(std::default_initializable<MyStruct>); // C++20
