#include "std.hpp"


class Crc32ComputeTest : public ::testing::Test
{
public:
    const static size_t size = 16;
    uint8_t buffer[size];

    void SetUp()
    {
        const char * str = "Crc32ComputeTest";
        memcpy(buffer, str, size );
    }
};

TEST_F(Crc32ComputeTest, NormalBuffer) {
    EXPECT_EQ(0xBFC870D0, Fw::Crypto::crc32(buffer, size));
}

TEST_F(Crc32ComputeTest, SomeCalls_crc32_update) {
    uint32_t crc = 0;
    size_t blockSize = 4;
    for (size_t sz = 0; sz < size; sz += blockSize) {
        size_t t = std::min(blockSize, size-sz);
        crc = Fw::Crypto::crc32_update(crc, &buffer[sz], t);
    }
    EXPECT_EQ(0xBFC870D0, crc);
}
