#include <gtest/gtest.h>
#include <Date.hpp>

TEST(Date, convert) {
    Date date("2018-09-10/12:00");
    EXPECT_EQ(date, Date(2018, 9, 10, 12, 00));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}
