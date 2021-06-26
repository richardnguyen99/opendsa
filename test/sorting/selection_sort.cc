/**
 * @file test/sorting/selection_sort.cc
 * @author Richard Nguyen 
 * @brief Testing cases for selection sorting implementation
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

TEST(SelectionSortingTest, SortedAssertion)
{
    std::vector<int> sorted_array = {1, 2, 3, 4, 5, 6};
    opendsa::selection_sort(sorted_array.begin(), sorted_array.end());

    EXPECT_THAT(sorted_array, ElementsAre(1, 2, 3, 4, 5, 6));
}

TEST(SelectionSortingTest, ReverseSortedAssertion)
{
    std::vector<int> rsorted_array = {7, 6, 5, 4, 3, 2, 1};
    opendsa::selection_sort(rsorted_array.begin(), rsorted_array.end());

    EXPECT_THAT(rsorted_array, ElementsAre(1, 2, 3, 4, 5, 6, 7));
}

TEST(SelectionSortingTest, RandomAssertion)
{
    std::vector<int> random_array = {3, 5, 2, 4, 1, 7, 6};
    opendsa::selection_sort(random_array.begin(), random_array.end());

    EXPECT_THAT(random_array, ElementsAre(1, 2, 3, 4, 5, 6, 7));
}
