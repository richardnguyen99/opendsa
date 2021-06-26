
/**
 * @file test/sorting/insertion_sort.cc
 * @author Richard Nguyen 
 * @brief Testing cases for insertion sorting implementation
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 */

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <opendsa/sorting.h>

using ::testing::ElementsAre;

TEST(InsertionSortingTest, SortedAssertion)
{
    std::vector<int> sorted_array = {1, 2, 3, 4, 5, 6};
    opendsa::insertion_sort(sorted_array.begin(), sorted_array.end());

    EXPECT_THAT(sorted_array, ElementsAre(1, 2, 3, 4, 5, 6));
}

TEST(InsertionSortingTest, ReverseSortedAssertion)
{
    std::vector<int> rsorted_array = {7, 6, 5, 4, 3, 2, 1};
    opendsa::insertion_sort(rsorted_array.begin(), rsorted_array.end());

    EXPECT_THAT(rsorted_array, ElementsAre(1, 2, 3, 4, 5, 6, 7));
}

TEST(InsertionSortingTest, RandomAssertion)
{
    std::vector<int> random_array = {3, 5, 2, 4, 1, 7, 6};
    opendsa::insertion_sort(random_array.begin(), random_array.end());

    EXPECT_THAT(random_array, ElementsAre(1, 2, 3, 4, 5, 6, 7));
}
