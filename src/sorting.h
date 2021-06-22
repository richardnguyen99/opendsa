#pragma once

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
     * @param begin 
     * @param end 
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
}