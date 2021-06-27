/**
 * @file sorting.h
 * @author Richard Nguyen
 * @brief Implementations for sorting algorithms
 * 
 * This header contains several implementations for 
 * sorting algorithms that only work with iterators.
 * 
 * @version 0.1
 * @date 2021-06-23
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

namespace opendsa
{
    /**
     * @brief Sort containers using the bubble sorting techinque
     * 
     * The algorithm will compare two adjacent values in the container
     * and swap them if necessary according to the specified order.
     * 
     * @param begin     beginning iterator of the container (`std::begin(container)`)
     * @param end       end iteraotr of the container (`std::end(container)`)
     */
    template <typename Iter>
    void bubble_sort(Iter begin, Iter end)
    {
        // Flag to break the algorithm when there is no
        // swapping inside, meaning the container is sorted.
        bool swapping = true;

        while (begin != end-- && swapping)
        {
            swapping = false;
            for (Iter current = begin; current != end; ++current)
            {
                if (*(current + 1) < *current)
                {
                    std::iter_swap(current, current + 1);
                    swapping = true;
                }
            }
        }
    }

    /**
     * @brief  Sort containers using the insertion sorting technique
     * 
     * The algorithm iterates through the array. For each iterator, it
     * will compare the current element with its predecessor. Continue 
     * to do that if the current element is in the right position
     * 
     * @param begin     beginning iterator of the container (`std::begin(container)`)
     * @param end       end iterator of the container (`std::end(container)`)
     */
    template <typename Iter>
    void insertion_sort(Iter begin, Iter end)
    {
        if (begin == end)
        {
            return;
        }

        // Traverse through the elememnt
        for (Iter next = std::next(begin); next != end; ++next)
        {

            // Check the current key
            for (Iter current = next; current > begin; --current)
            {
                // Predecessors
                Iter prev = std::prev(current);

                if (*current < *prev)
                {
                    std::iter_swap(current, prev);
                }
                else
                {
                    break;
                }
            }
        }
    }

    /**
     * @brief Sort containers using the selection sorting technique
     * 
     * The algorithm iterates through the array. For each iterator, 
     * it will find the smallest then swap with the current element.
     * 
     * @param begin     beginning iterator of the container (`std::begin(container)`)
     * @param end       end iterator of the container (`std::end(container)`)
     */
    template <typename Iter>
    void selection_sort(Iter begin, Iter end)
    {
        bool swapping = true;

        for (; begin != end && swapping; begin++)
        {
            swapping = false;
            Iter min = begin;

            // Maintain two sub arrays, one is sorted and one is not.
            for (Iter current = std::next(begin); current != end; current++)
            {
                // Find the smallest unitl it hits the end of
                // the unsorted array
                if (*current < *min)
                {
                    min = current;
                }
            }

            // At the end of the unsorted sub-array, swap the smallest
            // with the current index to put it into the sorted sub-array.
            if (min != end)
            {
                swapping = true;
                std::iter_swap(begin, min);
            }
        }
    }

    /**
     * @brief Sort containers using the quick sorting technique
     * 
     * The algorithm iterates through the array. For each iterator,
     * it will choose the first element as the pivot key, then it 
     * moves every elements that is less then the pivot key to the
     * left of the pivot and greater ones to the right. The process
     * will be repeated on the two halves of the array.
     * 
     * @tparam Iter 
     * @param begin 
     * @param end 
     */
    template <typename Iter>
    void quick_sort(Iter begin, Iter end)
    {
        if (begin == end)
            return;

        // Select the beginning iterator as pivot
        Iter pivot = begin;

        // Set the bound to later place the pivot
        Iter bound = std::next(pivot);

        for (Iter next = std::next(begin); next < end; ++next)
        {
            // Move the smaller elements to the left
            // The greater ones remain the same.
            if (*next < *pivot)
            {
                std::iter_swap(next, bound);
                bound++;
            }
        }

        // Swap the pivot key with the bound
        // The bound is the correct position
        // for the pivot after each round
        std::iter_swap(begin, bound - 1);

        // Repeat the process on the two sub containers
        quick_sort(begin, bound - 1);
        quick_sort(bound, end);
    }

    /**
     * @brief 
     * 
     * @tparam IterIn 
     * @tparam IterOut 
     * @param first_in 
     * @param first_out 
     * @param second_in 
     * @param second_out 
     * @param dest 
     */
    template <typename IterIn, typename IterOut>
    void merge(IterIn first_in, IterIn first_out,
               IterIn second_in, IterIn second_out, IterOut dest)
    {
        for (; first_in != first_out; ++dest)
        {
            if (second_in == second_out)
            {
                std::copy(first_in, first_out, dest);
                break;
            }

            if (*first_in < *second_in)
            {
                *dest = *first_in;
                ++first_in;
            }
            else
            {
                *dest = *second_in;
                ++second_in;
            }
        }

        std::copy(second_in, second_out, dest);
    }

    /**
     * @brief  Sorting containers using the merge sort technique
     * 
     * The algorithm divides the container into 2 smaller ones, until
     * the sub-containers cannot be divided anymore. Then, it merges
     * the sub containers using the merge function
     * 
     * @param begin 
     * @param end 
     */
    template <typename Iter>
    void merge_sort(Iter begin, Iter end)
    {
        if (std::distance(begin, end) > 1)
        {
            // Divide the container into 2 smaller containers
            Iter middle = std::next(begin, std::distance(begin, end) / 2);

            // Divide until there is only one element in the sub container
            merge_sort(begin, middle);
            merge_sort(middle, end);

            // Create a temp container to temporarily store the merged values
            std::vector<typename std::iterator_traits<Iter>::value_type> temp;

            // Merge the two containers then store in temp container
            merge(begin, middle, middle, end, std::back_inserter(temp));

            // Copy the temp to the original container
            std::copy(std::begin(temp), std::end(temp), begin);
        }
    }

    template <typename Iter>
    void heap_sort(Iter begin, Iter end)
    {
    }

    /**
     * @brief Sorting containers using the couting sort technique
     * 
     * The algorithm counts the number of objects having distinct 
     * key values in a way to tell the frequency the according
     * values appear in the container. Then it modifies the storage
     * to indicate at which elements should be in the output.
     * 
     * @param begin 
     * @param end 
     */
    template <typename Iter>
    void counting_sort(Iter begin, Iter end)
    {
        // Calculate the range of the count container
        const auto min = *(std::min_element(begin, end));
        const auto max = *(std::max_element(begin, end));

        // Initialize the count container with 0
        std::vector<std::size_t> count(max - min + 1, 0);

        // Count how often a unique object occurs
        std::for_each(begin, end, [&](auto x)
                      { ++count[x - min]; });

        // Modify the count container
        for (auto cursor = std::begin(count); cursor != std::end(count); ++cursor)
        {
            // Index will help to get the current element in the actual container
            auto idx = std::distance(std::begin(count), cursor);

            // Replace the current iterator with sorted value
            std::fill_n(begin, *cursor, idx + min);

            // Move the begin iterator so it doesn't collapse
            std::advance(begin, *cursor);
        }
    }
}