/**
 * @file deque.cpp
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Unittest for opendsa::deque
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 */

#include <gtest/gtest.h>
#include <initializer_list>

#include "deque.h"

namespace
{
TEST(DequeTest, Constructor)
{
    opendsa::deque<int> d({1, 2, 3, 4});

    EXPECT_EQ(4, d.size());
}

} // namespace
