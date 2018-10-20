#include <gtest/gtest.h>
#include <Date.hpp>

TEST(Date, convert) {
    Date date("2018-09-10/12:00");
    EXPECT_EQ(date, Date(2018, 9, 10, 12, 00));
    EXPECT_EQ(Date::stringToDate("2018-09-10/12:00"),
              Date(2018, 9, 10, 12, 00));
    EXPECT_EQ(Date(0, 0, 0, 0, 0), Date::stringToDate("2018-9-10/12:00"));
    EXPECT_EQ(Date(0, 0, 0, 0, 0), Date::stringToDate("2018/9/10/12:00"));
    EXPECT_EQ(Date(0, 0, 0, 0, 0), Date::stringToDate("2001-02-30/12:00"));
    EXPECT_EQ(Date(0, 0, 0, 0, 0), Date::stringToDate("1900-02-29/12:00"));
    EXPECT_EQ(Date(2000, 2, 29, 12, 0), Date::stringToDate("2000-02-29/12:00"));
    EXPECT_EQ(Date(1018, 9, 10, 12, 0), Date::stringToDate("1018-09-10/12:00"));
}