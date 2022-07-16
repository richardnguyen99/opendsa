#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <vector>

#include "include/algorithm.h"
#include "include/deque.h"
#include "include/vector.h"

void test_raw_allocation()
{
    std::allocator<int> alloc;
    using traits_t = std::allocator_traits<decltype(alloc)>;

    opendsa::deque_iterator<int, int &, int *> iter;

    int  *start  = 0x0;
    int  *finish = 0x0;
    int **map    = 0x0;

    const std::size_t MAX_SIZE = opendsa::get_deque_buffer_size(sizeof(int));
    const std::size_t SIZE     = 8;

    start  = traits_t::allocate(alloc, MAX_SIZE);
    finish = start;

    for (std::size_t i = 0; i < SIZE; i++)

        traits_t::construct(alloc, start + i, int());
    finish++;

    map = &start;
    opendsa::deque_iterator<int, int &, int *> iter1(finish - 1, map);
    opendsa::deque_iterator<int, const int &, const int *> iter2 = iter1;
    std::cout << *iter2 << "\n";

    std::_Deque_iterator<int, int &, int *>             iter3(finish - 1, map);
    std::_Deque_iterator<int, const int &, const int *> iter4 = iter3;
    std::cout << *iter4 << "\n";

    traits_t::deallocate(alloc, start, MAX_SIZE);
}

template <typename T>
void test_get_deque_info(const opendsa::deque<T> deque)
{
    std::cout << "============ DEQUE INFO: ============\n";
    std::cout << "Size: " << deque.size() << "\n";
    std::cout << "Max size: " << deque.max_size() << "\n";
    std::cout << "Empty?: " << (deque.empty() ? "yes" : "no") << "\n";

    if (!deque.empty())
    {
        std::cout << "First element: " << deque.front() << "\n";
        std::cout << "Last elelement: " << deque.back() << "\n";
    }

    std::cout << "Elements (forward): { ";
    std::size_t i = 0;
    std::for_each(deque.cbegin(), deque.cend(),
                  [&i, &deque](const T &e)
                  {
                      std::cout << e << ((i < deque.size() - 1) ? ", " : " ");
                      i++;
                  });
    std::cout << "}\n";

    std::cout << "Elements (backward): { ";
    i = 0;
    std::for_each(deque.crbegin(), deque.crend(),
                  [&i, &deque](const T &e)
                  {
                      std::cout << e << ((i < deque.size() - 1) ? ", " : " ");
                      i++;
                  });
    std::cout << "}\n\n";
}

int main(int argc, const char **argv)
{
    // test_raw_allocation()

    opendsa::deque<int> d;
    opendsa::deque<int> d1(8, 8);

    int array[] = {1, 2, 3, 4, 5, 6, 7, 8};

    opendsa::deque<int> d2(std::begin(array), std::end(array));
    opendsa::deque<int> d3 = {1, 2, 3, 4, 5, 6, 7, 8};
    opendsa::deque<int> d4(d3);
    d4.emplace_front(0);
    d4.emplace_front(-1);
    d4.emplace_back(9);
    d4.emplace_back(11);
    d4.emplace(d4.cend() - 1, 10);
    d4.push_front(-3);
    d4.emplace(d4.cbegin() + 1, -2);
    d4.insert(d4.cend(), 13);
    d4.insert(d4.cend() - 1, 12);
    d4.insert(d4.cbegin(), -5);
    d4.insert(d4.cbegin() + 1, -4);
    d4.insert(d4.cend(), {14, 15, 16});
    d4.insert(d4.cbegin(), {-11, -10});
    d4.insert(d4.cbegin() + 2, {-9, -8, -7, -6});
    d4.insert(d4.cend(), {23, 24});
    d4.insert(d4.cend() - 2, {17, 18, 19, 20, 21, 22});

    test_get_deque_info(d);
    test_get_deque_info(d3);
    test_get_deque_info(d4);

    return 0;
}