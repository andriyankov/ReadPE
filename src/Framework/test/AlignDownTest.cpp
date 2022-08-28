#include "std.hpp"


TEST(AlignDownTest, whenValueIsBiggerFactorByOne) {
    const uint32_t factor = 0x200;
    const uint32_t value = factor + 1;
    auto actual = Math::alignDown(value, factor);
    EXPECT_EQ(0x200, actual);
}

TEST(AlignDownTest, whenValueIsLessFactorByOne) {
    const uint32_t factor = 0x200;
    const uint32_t value = factor - 1;
    auto actual = Math::alignDown(value, factor);
    EXPECT_EQ(0, actual);
}

TEST(AlignDownTest, whenValueAndFactorEquals) {
    const uint32_t factor = 0x200;
    const uint32_t value = factor;
    auto actual = Math::alignDown(value, factor);
    EXPECT_EQ(0x200, actual);
}
