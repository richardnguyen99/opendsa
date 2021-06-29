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

#include <opendsa/container.h>
#include <opendsa/container_util.h>

using ::testing::ElementsAre;

TEST(ContainerTest, InitialDefault)
{
    opendsa::static_array<int> my_arr;

    EXPECT_EQ(my_arr.max_size(), 8);
    EXPECT_EQ(my_arr.size(), 0);
}

TEST(ContainerTest, InitialSize)
{
    opendsa::static_array<int> my_arr(3);

    EXPECT_EQ(my_arr.max_size(), 3);
    EXPECT_EQ(my_arr.size(), 0);
}

TEST(ContainerTest, InitialList)
{
    opendsa::static_array<int> my_arr = {1, 2, 3, 4, 5, 6, 7, 8};

    EXPECT_EQ(my_arr.max_size(), 8);
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