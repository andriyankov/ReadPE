#include "std.hpp"


TEST(StringUtils_JoinTest, WhenListStringIsEmpty) {
    std::list<std::string> strings;
    EXPECT_TRUE(StringUtils::join(strings).empty());
}

TEST(StringUtils_JoinTest, WhenListStringWithOneString) {
    std::list<std::string> strings;
    strings.emplace_back("abc");
    EXPECT_EQ(std::string("abc"), StringUtils::join(strings));
}

TEST(StringUtils_JoinTest, WhenListStringWithSomeStrings) {
    std::list<std::string> strings;
    strings.emplace_back("a1");
    strings.emplace_back("b2");
    strings.emplace_back("c3");
    EXPECT_EQ(std::string("a1,b2,c3"), StringUtils::join(strings));
}

TEST(StringUtils_JoinTest, WhenListStringWithCustomDelimiter) {
    std::list<std::string> strings;
    strings.emplace_back("a1");
    strings.emplace_back("b2");
    strings.emplace_back("c3");
    EXPECT_EQ(std::string("a1; b2; c3"), StringUtils::join(strings, "; "));
}
