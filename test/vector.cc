/**
 * @file vector.cpp
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Unittest for opendsa::vector
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 */

#include <gtest/gtest.h>
#include <initializer_list>

#include "vector.h"

namespace
{
TEST(VectorTest, Constructor)
{
    opendsa::vector<int> v({1, 2, 3, 4});

    EXPECT_EQ(4, v.size());
}

} // namespace
