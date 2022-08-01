#include <algorithm>
#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <vector>

#include "singly_list.h"

template <typename T>
void
print(const opendsa::singly_list<T> &list)
{
    std::cout << "{ ";
    std::for_each(list.cbegin(), list.cend(),
                  [](const auto &e) { std::cout << e << " -> "; });
    std::cout << "null }\n";
}

template <typename T>
void
test_list_info(const opendsa::singly_list<T> &list,
               const std::string &list_name)
{
    std::cout << "========= Test list: " << list_name << " =========\n";

    std::cout << "- Empty?: " << (list.empty() ? "yes" : "no") << "\n";
    std::cout << "- Front: " << (!list.empty() && list.front()) << "\n";
    std::cout << "- Max possible nodes: " << list.max_size() << "\n";

    std::cout << "- Contents: \n  ";
    print(list);

    std::cout << "\n";
}

template <typename T>
void
test_list_insert_after(opendsa::singly_list<T> &list, const T &value,
                       std::ptrdiff_t pos, const std::string &list_name)
{
    std::cout << "========= Test " << list_name << "::insert_after(" << pos
              << ", " << value << ") =========\n";

    typename opendsa::singly_list<T>::const_iterator iter;

    if (pos == -1)
        iter = list.cbefore_begin();
    else
    {
        iter = list.cbegin();
        for (; pos; --pos, ++iter)
        {
        }
    }

    std::cout << "- Before inserting: \n  ";
    print(list);

    list.insert_after(iter, value);

    std::cout << "- After inserting: \n  ";
    print(list);

    std::cout << "\n";
}

template <typename T>
void
test_list_insert_after(opendsa::singly_list<T> &list, std::size_t count,
                       const T &value, std::ptrdiff_t pos,
                       const std::string &list_name)
{
    std::cout << "========= Test " << list_name << "::insert_after(" << pos
              << ", " << count << ", " << value << ") =========\n";

    typename opendsa::singly_list<T>::const_iterator iter;

    if (pos == -1)
        iter = list.cbefore_begin();
    else
    {
        iter = list.cbegin();
        for (; pos; --pos, ++iter)
        {
        }
    }

    std::cout << "- Before inserting: \n  ";
    print(list);

    list.insert_after(iter, count, value);

    std::cout << "- After inserting: \n  ";
    print(list);

    std::cout << "\n";
}

template <typename T, typename InputIter>
void
test_list_insert_after(opendsa::singly_list<T> &list, InputIter first,
                       InputIter end, std::ptrdiff_t pos,
                       const std::string &list_name)
{
    std::cout << "========= Test " << list_name
              << "::insert_after() range =========\n";

    typename opendsa::singly_list<T>::const_iterator iter;

    if (pos == -1)
        iter = list.cbefore_begin();
    else
    {
        iter = list.cbegin();
        for (; pos; --pos, ++iter)
        {
        }
    }

    std::cout << "- Before inserting: \n  ";
    print(list);

    list.insert_after(iter, first, end);

    std::cout << "- After inserting: \n  ";
    print(list);

    std::cout << "\n";
}

template <typename T>
void
test_list_insert_after(opendsa::singly_list<T> &list,
                       std::initializer_list<T> ilist, std::ptrdiff_t pos,
                       const std::string &list_name)
{
    std::cout << "========= Test " << list_name << "::insert_after(" << pos
              << ", { ";
    std::for_each(ilist.begin(), ilist.end(),
                  [](const auto &e) { std::cout << e << ", "; });
    std::cout << "}) =========\n";

    typename opendsa::singly_list<T>::const_iterator iter;

    if (pos == -1)
        iter = list.cbefore_begin();
    else
    {
        iter = list.cbegin();
        for (; pos; --pos, ++iter)
        {
        }
    }

    std::cout << "- Before inserting: \n  ";
    print(list);

    list.insert_after(iter, ilist);

    std::cout << "- After inserting: \n  ";
    print(list);

    std::cout << "\n";
}

int
main(int argc, const char **argv)
{
    opendsa::singly_list<int> sgl_list1(8, 4);
    opendsa::singly_list<int> sgl_list2({1, 2, 3, 4, 5, 6, 7, 8});
    opendsa::singly_list<int> sgl_list3(sgl_list1.begin(), sgl_list1.end());
    opendsa::singly_list<int> sgl_list4(sgl_list2);
    opendsa::singly_list<int> sgl_list5(std::move(sgl_list2));
    opendsa::singly_list<int> sgl_list6({1, 2, 3, 4, 9, 10, 11, 12});

    test_list_info(sgl_list1, "singly-linked list 1");
    test_list_info(sgl_list2, "singly-linked list 2");
    test_list_info(sgl_list3, "singly-linked list 3");
    test_list_info(sgl_list4, "singly-linked list 4");
    test_list_info(sgl_list5, "singly-linked list 5");

    test_list_insert_after(sgl_list5, 0, -1, "sgl_list5");
    test_list_insert_after(sgl_list5, 9, 8, "sgl_list5");

    test_list_insert_after(sgl_list1, 8, 3, -1, "sgl_list1");
    test_list_insert_after(sgl_list1, 8, 5, 15, "sgl_list1");
    test_list_insert_after(sgl_list1, 4, 7, 23, "sgl_list1");
    test_list_insert_after(sgl_list1, 4, 6, 23, "sgl_list1");

    test_list_insert_after(sgl_list5, sgl_list3.begin(), sgl_list3.end(), -1,
                           "sgl_list5");
    test_list_insert_after(sgl_list5, sgl_list3.begin(), sgl_list3.end(), 8,
                           "sgl_list5");

    test_list_insert_after(sgl_list6, {5, 6, 7, 8}, 3, "sgl_list6");
    test_list_insert_after(sgl_list6, {13, 14, 15, 16}, 11, "sgl_list6");
    test_list_insert_after(sgl_list6, {-3, -2, -1, 0}, -1, "sgl_list6");
    test_list_insert_after(sgl_list6, {}, -1, "sgl_list6");

    return 0;
}
