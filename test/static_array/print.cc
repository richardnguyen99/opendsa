/**
 * @file test/container/print.cc
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

TEST(StaticArrayTest, NormalIterator)
{
    opendsa::static_array<int> my_arr = {1, 2, 3, 4, 5, 6, 7, 8};

    int i = 1;
    for (auto &e : my_arr)
    {
        EXPECT_EQ(e, i++);
    }
}

TEST(StaticArrayTest, ConstIterator)
{
    opendsa::static_array<int> my_arr = {1, 2, 3, 4, 5, 6, 7, 8};

    int i = 1;
    for (const auto &e : my_arr)
    {
        EXPECT_EQ(e, i++);
    }
}

TEST(StaticArrayTest, NormalReverseIterator)
{
    opendsa::static_array<int> my_arr = {1, 2, 3, 4, 5, 6, 7, 8};

    int i = 8;
    for (auto cursor = std::rbegin(my_arr); cursor != std::rend(my_arr);
         ++cursor)
    {
        EXPECT_EQ(*cursor, i--);
    }
}

TEST(StaticArrayTest, ConstReverseIterator)
{
    opendsa::static_array<int> my_arr = {1, 2, 3, 4, 5, 6, 7, 8};

    int i = 8;
    std::for_each(std::crbegin(my_arr), std::crend(my_arr),
                  [&i](const auto &e) { EXPECT_EQ(e, i--); });
}
