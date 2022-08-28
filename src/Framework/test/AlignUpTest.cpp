#include "std.hpp"


TEST(AlignUpTest, whenValueIsBiggerFactorByOne) {
    const uint32_t factor = 0x200;
    const uint32_t value = factor + 1;
    auto actual = Math::alignUp(value, factor);
    EXPECT_EQ(0x400, actual);
}

TEST(AlignUpTest, whenValueIsLessFactorByOne) {
    const uint32_t factor = 0x200;
    const uint32_t value = factor - 1;
    auto actual = Math::alignUp(value, factor);
    EXPECT_EQ(0x200, actual);
}

TEST(AlignUpTest, whenValueAndFactorEquals) {
    const uint32_t factor = 0x200;
    const uint32_t value = factor;
    auto actual = Math::alignUp(value, factor);
    EXPECT_EQ(0x200, actual);
}
