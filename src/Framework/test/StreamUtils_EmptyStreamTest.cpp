#include "std.hpp"


class StreamUtils_EmptyStreamTest : public ::testing::Test
{
protected:
    std::stringstream testStream;
};


TEST_F(StreamUtils_EmptyStreamTest, GetSizeOfEmptyStream) {
    //Arrange
    std::streamsize sz = Io::StreamUtils::getSize(testStream);

    //Assert
    EXPECT_EQ(0, sz);
}

TEST_F(StreamUtils_EmptyStreamTest, ReadEmptyStream) {
    //Arrange
    enum { invalidPos = 5 };
    std::string emptyString = Io::StreamUtils::readStringAt(testStream, invalidPos);

    //Assert
    EXPECT_TRUE(emptyString.empty());
}
