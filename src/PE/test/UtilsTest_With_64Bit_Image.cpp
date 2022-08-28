#include "std.hpp"


class UtilsTest_With_64Bit_Image : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        fixture = ::Test::Fixture<pe::arch64>::create("withSectionHeadersOnly64");
    }

protected:
    ::Test::Fixture<pe::arch64>::FixturePtr_t fixture;
};


TEST_F(UtilsTest_With_64Bit_Image, RvaToSectionIndex) {
    auto& image = fixture->getImage();
    EXPECT_EQ(0, pe::utils::RvaToSectionIndex(image, 0x11FF));
    EXPECT_EQ(1, pe::utils::RvaToSectionIndex(image, 0x2000));
    EXPECT_EQ(1, pe::utils::RvaToSectionIndex(image, 0x21FF));
    EXPECT_EQ(2, pe::utils::RvaToSectionIndex(image, 0x3000));
}

TEST_F(UtilsTest_With_64Bit_Image, RawToSectionIndex) {
    auto& image = fixture->getImage();
    EXPECT_EQ(0, pe::utils::RawToSectionIndex(image, 0x3FF));
    EXPECT_EQ(1, pe::utils::RawToSectionIndex(image, 0x400));
    EXPECT_EQ(1, pe::utils::RawToSectionIndex(image, 0x5FF));
    EXPECT_EQ(2, pe::utils::RawToSectionIndex(image, 0x600));
}

TEST_F(UtilsTest_With_64Bit_Image, GetLastFileSection) {
    auto& image = fixture->getImage();
    EXPECT_EQ(2, pe::utils::getLastFileSection(image));
}

TEST_F(UtilsTest_With_64Bit_Image, GetLastVirtualSection) {
    auto& image = fixture->getImage();
    EXPECT_EQ(2, pe::utils::getLastVirtualSection(image));
}
