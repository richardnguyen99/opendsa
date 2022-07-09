/**
 * @file algorithm.h
 * @author Richard Nguyen (richard.ng0616@.com)
 * @brief Algorithm functions to work with C++ containers
 * @version 0.1
 * @date 2022-07-08
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_ALGO_H
#define __OPENDSA_ALGO_H 1

#include <concepts>
#include <iterator>

namespace opendsa
{
    template <typename Container>
    concept RequireSequenceContainer = requires(Container c)
    {
        std::begin(c);
        std::end(c);
        c.size();
        {
            c.size()
            } -> std::same_as<std::size_t>;
        c[0];
    };

    /**
     * @brief Median of two sorted array
     *
     * @param nums1 First sequential container of numbers
     * @param nums2 Second sequential container of numbers
     *
     * This function will compute the the median of two given sorted arrays of
     * numeric values. If either of the arrays is not sorted, the output will be
     * incorrect.
     */
    template <typename Container>
    requires RequireSequenceContainer<Container>
    double median(Container __nums1, Container __nums2)
    {
        std::size_t __total_size = __nums1.size() + __nums2.size();
        std::size_t __i1 = 0, __i2 = 0;
        std::size_t __prev = 0, __curr = 0, __result = 0;

        // This constraint will make sure that the runtime compplexity will be
        // O(log(m + n)), where m and n are the size of the container nums1 and
        // nums2 respectively.
        while (__result++ <= (__total_size / 2))
        {
            // In case the size of merged container is even.
            __prev = __curr;

            if (__i1 >= __nums1.size())
            {
                __curr = __nums2[__i2++];
                continue;
            }

            if (__i2 >= __nums2.size())
            {
                __curr = __nums1[__i1++];
                continue;
            }

            if (__nums1[__i1] < __nums2[__i2])
                __curr = __nums1[__i1++];
            else
                __curr = __nums2[__i2++];
        }

        return (__total_size % 2 == 1) ? (double)__curr
                                       : ((double)(__prev + __curr) / 2);
    }
}; // namespace opendsa

#endif
