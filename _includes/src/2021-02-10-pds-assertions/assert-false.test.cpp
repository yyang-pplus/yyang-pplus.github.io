#include "assert-false.hpp"

#include <gtest/gtest.h>


TEST(UniqueArrayTests, TestSanity) {
    UniqueArray unique_array;

    EXPECT_EQ(0, unique_array.Insert(0));
    EXPECT_EQ(1, unique_array.Insert(1));
    EXPECT_EQ(2, unique_array.Insert(2));
    EXPECT_EQ(0, unique_array.Insert(0));
    EXPECT_EQ(1, unique_array.Insert(1));
    EXPECT_EQ(2, unique_array.Insert(2));
    EXPECT_EQ(3, unique_array.Insert(3));
    EXPECT_EQ(4, unique_array.Insert(4));
    EXPECT_EQ(2, unique_array.Insert(2));
    EXPECT_EQ(0, unique_array.Insert(0));
}
