#include "std.hpp"


class AlignerTest_WhenAlignmentUse2 : public ::testing::Test
{
public:
    const uint32_t FILE_ALIGNMENT = 0x1000;
    const uint32_t SECTION_ALIGNEMENT = 0x1000;
    const uint32_t FORCED_FILE_ALIGNMENT = 0x200;

public:
    virtual void SetUp()
    {
        aligner = std::make_unique<Aligner>(FILE_ALIGNMENT, SECTION_ALIGNEMENT);
    }

protected:
    std::unique_ptr<Aligner> aligner;
};


TEST_F(AlignerTest_WhenAlignmentUse2, GetFileOffset_FileAlignmentMinusOne) {
    auto offset = FORCED_FILE_ALIGNMENT-1;
    EXPECT_EQ(0, aligner->getFileOffset(offset));
}

TEST_F(AlignerTest_WhenAlignmentUse2, GetFileOffset_FileAlignmentPlusOne) {
    auto offset = FORCED_FILE_ALIGNMENT+1;
    EXPECT_EQ(FORCED_FILE_ALIGNMENT, aligner->getFileOffset(offset));
}

TEST_F(AlignerTest_WhenAlignmentUse2, GetFileOffset_EqualFileAlignment) {
    auto offset = FORCED_FILE_ALIGNMENT;
    EXPECT_EQ(FORCED_FILE_ALIGNMENT, aligner->getFileOffset(offset));
}

TEST_F(AlignerTest_WhenAlignmentUse2, getVirtualAddress_SectionAlignmentMinusOne) {
    auto address = SECTION_ALIGNEMENT-1;
    EXPECT_EQ(0, aligner->getVirtualAddress(address));
}

TEST_F(AlignerTest_WhenAlignmentUse2, getVirtualAddress_SectionAlignmentPlusOne) {
    auto address = SECTION_ALIGNEMENT+1;
    EXPECT_EQ(SECTION_ALIGNEMENT, aligner->getVirtualAddress(address));
}

TEST_F(AlignerTest_WhenAlignmentUse2, getVirtualAddress_EqualSectionAlignment) {
    auto offset = SECTION_ALIGNEMENT;
    EXPECT_EQ(SECTION_ALIGNEMENT, aligner->getFileOffset(offset));
}

TEST_F(AlignerTest_WhenAlignmentUse2, getVirtualSize_AlignedSizeMinusOne) {
    auto size = 0x27FFF;
    EXPECT_EQ(0x28000, aligner->getVirtualSize(size));
}

TEST_F(AlignerTest_WhenAlignmentUse2, getVirtualSize_AlignedSize) {
    auto size = 0x28000;
    EXPECT_EQ(0x28000, aligner->getVirtualSize(size));
}

TEST_F(AlignerTest_WhenAlignmentUse2, getSectionSize) {
    ImgSectionHeader header;
    header.SizeOfRawData = 0x1F0;
    header.Misc.VirtualSize = 0x27FFF;
    EXPECT_EQ(FILE_ALIGNMENT, aligner->getSectionSize(header));
}
