/**
 * @file container_util.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Helper headers for working with container headers
 * @version 0.1
 * @date 2021-06-28
 *
 * @copyright Copyright (c) 2021
 */

#include "container.h"
#include <algorithm>

using namespace opendsa;

namespace std
{
    template <std::size_t index, class T>
    constexpr T &get(static_array<T> &arr) noexcept
    {
        return arr[index];
    }
} // namespace std
