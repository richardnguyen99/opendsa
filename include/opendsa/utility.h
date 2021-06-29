#pragma once

#include <ostream>
#include <vector>

namespace opendsa
{
    /**
     * @brief General generator with specified container size
     *
     * The function creates a random vector of objects of type T
     * according the specified generator passed in.
     *
     * @param size Desired size of the container
     * @param gen Specified generator to tell how the generating works
     * @return std::vector<T>
     */
    template <typename T, typename _Generator>
    std::vector<T> gen_randarr(std::size_t size, _Generator gen)
    {
        std::vector<T> arr;

        for (std::size_t i = 0; i < size; ++i)
        {
            // generated type should match the type of container
            arr.push_back(gen());
        }

        return arr;
    }

    std::vector<int> gen_randiarr(std::size_t size, std::size_t range);
    std::vector<int> gen_randiarr(std::size_t size, std::size_t lower,
                                  std::size_t upper);

    /**
     * @brief Helper overloaded function to print out vector to
     * the terminal
     *
     * @param out
     * @param container
     * @return std::ostream&
     */
    template <typename T>
    std::ostream &operator<<(std::ostream &out, const std::vector<T> &container)
    {
        out << "[";

        // Make sure the implemtation for printing user-defined
        // data type exists
        for (auto e : container)
        {
            out << " " << e;
        }
        out << " ]" << std::endl;
    }
} // namespace opendsa