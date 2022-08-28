#include "std.hpp"


TEST(SectionDetailsTest, DecodeSectionFlags32) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("with22SectionsImg32");
    auto sectionsDetails = pe:: parse::Sections(fixture->getImage());

    enum { numSectionsCount = 22 };
    EXPECT_EQ(numSectionsCount, sectionsDetails.size());

    struct ExpectedDecodeFlag 
    {
        int index;
        const char * flags;
    };

    const ExpectedDecodeFlag expectedFlags[] =
    {
        {0, "code,not pageable,executable,readable"},
        {4, "initialized,not pageable,readable,writeable"},
        {7, "code,executable,readable"},
        {13, "uninitialized,readable,writeable"},
        {14, "initialized,readable"},
        {15, "initialized,readable,writeable"},
        {19, "code,can be discarded,executable,readable,writeable"},
        {21, "initialized,can be discarded,readable"}
    };

    int actualIndex = 0;
    int expectedIndex = 0;
    for (const auto& actual : sectionsDetails)  {
        // Act
        if (actualIndex != expectedFlags[expectedIndex].index) {
            actualIndex++;
            continue;
        }
        auto actualFlags = Fw::StringUtils::join(actual.flags, ",");

        // Assert
        EXPECT_EQ(expectedFlags[expectedIndex].flags, actualFlags);
        actualIndex++;
        expectedIndex++;
    }
}
