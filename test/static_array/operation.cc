/**
 * @file test/container/operation.cc
 * @author Richard Nguyen
 * @brief Testing cases for intializing container header
 * @version 0.1
 * @date 2021-06-29
 *
 * @copyright Copyright (c) 2021
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opendsa/static_array.h>

using ::testing::ElementsAre;

TEST(StaticArrayTest, EqualCompare)
{
    opendsa::static_array<int> arr1 = {1, 2, 3, 4, 5, 6, 7, 8};
    opendsa::static_array<int> arr2 = {1, 2, 3, 4, 5, 6, 7, 8};

    EXPECT_EQ(arr1, arr2);
}

TEST(StaticArrayTest, NotEqualCompare)
{
    opendsa::static_array<int> arr1 = {1, 2, 3, 4, 5, 6, 7, 8};
    opendsa::static_array<int> arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    EXPECT_EQ(arr1, arr2);
}

TEST(StaticArrayTest, FillOperation)
{
    opendsa::static_array<int> arr1;
    arr1.fill(2);
    opendsa::static_array<int> arr2 = {2, 2, 2, 2, 2, 2, 2, 2};

    EXPECT_EQ(arr1, arr2);
}

TEST(StaticArrayTest, SwapOperation)
{
    opendsa::static_array<int> arr1 = {1, 2, 3, 4, 5, 6, 7, 8};

    opendsa::static_array<int> arr2 = {3, 2, 1, 4, 5, 6, 7, 8};
    opendsa::static_array<int> tmp  = {3, 2, 1, 4, 5, 6, 7, 8};
    arr1.swap(arr2);

    EXPECT_EQ(arr1, tmp);
}
