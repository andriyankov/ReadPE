#include "std.hpp"
#include "gtest/gtest-all.cc"


int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
