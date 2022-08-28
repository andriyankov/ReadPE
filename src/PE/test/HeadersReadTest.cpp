#include "std.hpp"


TEST(HeadersReadTest, IncompleteDosHeader) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("incompleteDosHeader");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); } , CorruptedException);
}

TEST(HeadersReadTest, IncompleteNTHeaders) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("incompleteNTHeaders");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); }, CorruptedException);
}

TEST(HeadersReadTest, IncompleteSectionHeaders32) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("incompleteSectionHeaders32");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); } , CorruptedException);
}

TEST(HeadersReadTest, IncompleteSectionHeaders64) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch64>::create("incompleteSectionHeaders64");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); }, CorruptedException);
}

TEST(HeadersReadTest, IncorrectDosSignature) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("incorrectDosSignature");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); }, FormatException);
}

TEST(HeadersReadTest, IncorrectNtSignature) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("incorrectNTSignature");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); }, FormatException);
}

TEST(HeadersReadTest, IncorrectOptHdrMagic32) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch32>::create("incorrectOptionalHeaderMagic32");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); }, FormatException);
}

TEST(HeadersReadTest, IncorrectOptHdrMagic64) {
    // Arrange
    auto fixture = ::Test::Fixture<pe::arch64>::create("incorrectOptionalHeaderMagic64");
    // Act and Assert
    EXPECT_THROW({ fixture->getImage().read(); }, FormatException);
}
