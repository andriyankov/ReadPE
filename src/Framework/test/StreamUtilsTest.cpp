#include "std.hpp"


class StreamUtilsTest : public ::testing::Test
{
protected:
    std::stringstream testStream;

    virtual void SetUp()
    {
        const uint8_t test_data[] = {
            0x00, 0x31, 0x00, 0x6E, 0x6F, 0x72, 0x6D, 0x61, 
            0x6C, 0x2D, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 
            0x00, 0x62, 0x69, 0x6E, 0xC8, 0x72, 0x79, 0x2D, 
            0x73, 0x74, 0x72, 0xF9, 0x6E, 0x67, 0x00, 0x73, 
            0x74, 0x72, 0x69, 0x6E, 0x67, 0x2D, 0x61, 0x74, 
            0x2D, 0x65, 0x6E, 0x64, 0x00, 0x0A
        };

        size_t len = sizeof(test_data) / sizeof(*test_data);
        std::string str(reinterpret_cast<const char*>(test_data), len);
        testStream << str;
    }
};


TEST_F(StreamUtilsTest, GetSizeOfStream)
{
    //Arrange
    std::streamsize sz = Io::StreamUtils::getSize(testStream);

    //Assert
    EXPECT_EQ(0x2E, sz);
}

TEST_F(StreamUtilsTest, ReadNormalString) {
    //Arrange
    enum { stringPos = 3 };
    std::string str = Io::StreamUtils::readStringAt(testStream, stringPos);

    //Assert
    EXPECT_EQ("normal-string", str);
}

TEST_F(StreamUtilsTest, ReadOneChar) {
    //Arrange
    enum { charPos = 0x0F };
    std::string str = Io::StreamUtils::readStringAt(testStream, charPos);

    //Assert
    EXPECT_EQ("g", str);
}

TEST_F(StreamUtilsTest, ReadStringWithBinaryData) {
    //Arrange
    enum { stringPos = 0x11 };
    std::string str = Io::StreamUtils::readStringAt(testStream, stringPos);

    //Assert
    EXPECT_EQ("bin\xC8ry-str\xF9ng", str);
}

TEST_F(StreamUtilsTest, ReadNullByte) {
    //Arrange
    enum { nullBytePos = 2 };
    std::string emptyString = Io::StreamUtils::readStringAt(testStream, nullBytePos);

    //Assert
    EXPECT_TRUE(emptyString.empty());
}

TEST_F(StreamUtilsTest, ReadAtEnd) {
    //Arrange
    enum { endPos = 0x2E };
    std::string str = Io::StreamUtils::readStringAt(testStream, endPos);

    //Assert
    EXPECT_TRUE(str.empty());
}
