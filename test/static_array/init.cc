/**
 * @file test/container/init.cc
 * @author Richard Nguyen
 * @brief Testing cases for intializing container header
 * @version 0.1
 * @date 2021-06-24
 *
 * @copyright Copyright (c) 2021
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opendsa/static_array.h>

using ::testing::ElementsAre;

TEST(StaticArrayTest, InitialDefault)
{
    opendsa::static_array<int> my_arr;

    EXPECT_EQ(my_arr.size(), 8);
}

TEST(StaticArrayTest, InitialSize)
{
    opendsa::static_array<int> my_arr(3);

    EXPECT_EQ(my_arr.size(), 3);
}

TEST(StaticArrayTest, InitialList)
{
    opendsa::static_array<int> my_arr = {1, 2, 3, 4, 5, 6, 7, 8};

    EXPECT_EQ(my_arr.size(), 8);

    EXPECT_EQ(my_arr[0], 1);
    EXPECT_EQ(my_arr[1], 2);
    EXPECT_EQ(my_arr[2], 3);
    EXPECT_EQ(my_arr[3], 4);
    EXPECT_EQ(my_arr[4], 5);
    EXPECT_EQ(my_arr[5], 6);
    EXPECT_EQ(my_arr[6], 7);
    EXPECT_EQ(my_arr[7], 8);
}

TEST(StaticArrayTest, InitialStaticArray)
{
    opendsa::static_array<int> tmp    = {1, 2, 3, 4, 5, 6};
    opendsa::static_array<int> my_arr = tmp;

    EXPECT_EQ(my_arr.size(), 6);

    EXPECT_EQ(my_arr[0], 1);
    EXPECT_EQ(my_arr[1], 2);
    EXPECT_EQ(my_arr[2], 3);
    EXPECT_EQ(my_arr[3], 4);
    EXPECT_EQ(my_arr[4], 5);
    EXPECT_EQ(my_arr[5], 6);
}

TEST(StaticArrayTest, InitialManualWithNoSize)
{
    opendsa::static_array<int> tmp;
    tmp[0] = 1;
    tmp[1] = 2;
    tmp[2] = 3;
    tmp[3] = 4;
    tmp[4] = 5;
    tmp[5] = 6;

    EXPECT_EQ(tmp.size(), 8);

    EXPECT_EQ(tmp[0], 1);
    EXPECT_EQ(tmp[1], 2);
    EXPECT_EQ(tmp[2], 3);
    EXPECT_EQ(tmp[3], 4);
    EXPECT_EQ(tmp[4], 5);
    EXPECT_EQ(tmp[5], 6);
    EXPECT_EQ(tmp[6], 0);
}
