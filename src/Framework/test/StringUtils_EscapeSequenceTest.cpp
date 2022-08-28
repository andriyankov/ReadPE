#include "std.hpp"


TEST(StringUtils_EscapeSequenceTest, WithOneNonAscii) {
    unsigned char bytes[] = { 0x2 };
    int len = static_cast<int>(sizeof(bytes) / sizeof(*bytes));
    EXPECT_EQ(std::string("\\x02"), StringUtils::escape_sequence(bytes, len));
}

TEST(StringUtils_EscapeSequenceTest, WithAscii) {
    unsigned char bytes[] = {'.', 't', 'e', 'x', 't'};
    int len = static_cast<int>(sizeof(bytes) / sizeof(*bytes));
    EXPECT_EQ(std::string(".text"), StringUtils::escape_sequence(bytes, len));
}

TEST(StringUtils_EscapeSequenceTest, WithAsciiAndOneNonAscii) {
    unsigned char bytes[] = { '.', 't', 'e', 0x0, 'x', 't' };
    int len = static_cast<int>(sizeof(bytes) / sizeof(*bytes));
    EXPECT_EQ(std::string(".te\\x00xt"), StringUtils::escape_sequence(bytes, len));
}

TEST(StringUtils_EscapeSequenceTest, WhenFirstCharIsZero) {
    unsigned char bytes[] = { 0x0, '1','2','3','4','5','6','7' };
    int len = static_cast<int>(sizeof(bytes) / sizeof(*bytes));
    EXPECT_EQ(std::string("\\x001234567"), StringUtils::escape_sequence(bytes, len));
}

TEST(StringUtils_EscapeSequenceTest, WhenAllCharsAreZeroes) {
    unsigned char bytes[] = { 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    int len = static_cast<int>(sizeof(bytes) / sizeof(*bytes));
    auto expected = std::string("\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00");
    EXPECT_EQ(expected, StringUtils::escape_sequence(bytes, len));
}
