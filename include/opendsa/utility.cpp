#include "utility.h"
#include <random>
#include <vector>

std::vector<int> opendsa::gen_randiarr(std::size_t size, std::size_t range)
{
    std::mt19937                       gen{std::random_device{}()};
    std::uniform_int_distribution<int> arr_size{0, range};

    std::vector<int> iarr;
    for (size_t i = 0; i < size; ++i)
    {
        iarr.push_back(arr_size(gen));
    }

    return iarr;
}

std::vector<int> opendsa::gen_randiarr(std::size_t size, std::size_t lower,
                                       std::size_t upper)
{
    std::mt19937                       gen{std::random_device{}()};
    std::uniform_int_distribution<int> arr_size{lower, upper};

    std::vector<int> iarr;
    for (size_t i = 0; i < size; ++i)
    {
        iarr.push_back(arr_size(gen));
    }

    return iarr;
}