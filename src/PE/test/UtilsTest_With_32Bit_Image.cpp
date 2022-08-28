#include "std.hpp"


class UtilsTest_With_32Bit_Image : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        fixture = ::Test::Fixture<pe::arch32>::create("withSectionHeadersOnly32");
    }

protected:
    ::Test::Fixture<pe::arch32>::FixturePtr_t fixture;
};


TEST_F(UtilsTest_With_32Bit_Image, RvaToSectionIndex) {
    auto& image = fixture->getImage();
    EXPECT_EQ(0, pe::utils::RvaToSectionIndex(image, 0x11FF));
    EXPECT_EQ(1, pe::utils::RvaToSectionIndex(image, 0x2000));
    EXPECT_EQ(1, pe::utils::RvaToSectionIndex(image, 0x21FF));
    EXPECT_EQ(2, pe::utils::RvaToSectionIndex(image, 0x3000));
}

TEST_F(UtilsTest_With_32Bit_Image, RawToSectionIndex) {
    auto& image = fixture->getImage();
    EXPECT_EQ(0, pe::utils::RawToSectionIndex(image, 0x5FF));
    EXPECT_EQ(1, pe::utils::RawToSectionIndex(image, 0x600));
    EXPECT_EQ(1, pe::utils::RawToSectionIndex(image, 0x7FF));
    EXPECT_EQ(2, pe::utils::RawToSectionIndex(image, 0x800));
}

TEST_F(UtilsTest_With_32Bit_Image, GetLastFileSection) {
    auto& image = fixture->getImage();
    EXPECT_EQ(3, pe::utils::getLastFileSection(image));
}

TEST_F(UtilsTest_With_32Bit_Image, GetLastVirtualSection) {
    auto& image = fixture->getImage();
    EXPECT_EQ(3, pe::utils::getLastVirtualSection(image));
}
