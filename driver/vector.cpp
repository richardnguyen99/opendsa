#include <algorithm>
#include <iostream>
#include <vector>

#include "include/vector.h"

int main(int argc, const char **argv)
{
    opendsa::vector<int> vec1 = {1, 2, 3, 4, 5, 6, 7, 8};

    std::cout << "1st element: " << vec1.front() << "\n";
    std::cout << "last element: " << vec1.back() << "\n";
    std::cout << "Raw data: " << vec1.data() << "\n";
    std::cout << "Raw data at 1: " << *(vec1.data() + 1) << "\n";

    std::cout << "Iterator begin(): " << *(vec1.begin()) << "\n";
    std::cout << "Iterator last element: " << *(vec1.end()) << "\n";
    std::cout << "Iterator element at 1: " << *(vec1.begin()++) << "\n";

    for (const int &e : vec1)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";

    std::for_each(std::rbegin(vec1), std::rend(vec1),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";

    std::cout << "Old capacity: " << vec1.capacity() << "\n";
    vec1.reserve(16);
    std::cout << "New capacity: " << vec1.capacity() << "\n";

    std::cout << "1st element: " << vec1.front() << "\n";
    std::cout << "last element: " << vec1.back() << "\n";
    std::cout << "Raw data: " << vec1.data() << "\n";
    std::cout << "Raw data at 1: " << *(vec1.data() + 1) << "\n";

    std::cout << "Iterator begin(): " << *(vec1.begin()) << "\n";
    std::cout << "Iterator last element: " << *(vec1.end()) << "\n";
    std::cout << "Iterator element at 1: " << *(vec1.begin()++) << "\n";

    for (const int &e : vec1)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";

    std::for_each(std::rbegin(vec1), std::rend(vec1),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "Old capacity: " << vec1.capacity() << "\n";
    vec1.shrink_to_fit();
    std::cout << "New capacity: " << vec1.capacity() << "\n";
    std::cout << "1st element: " << vec1.front() << "\n";
    std::cout << "last element: " << vec1.back() << "\n";
    std::cout << "Raw data: " << vec1.data() << "\n";
    std::cout << "Raw data at 1: " << *(vec1.data() + 1) << "\n";

    vec1.reserve(16);
    std::cout << "Insert -1 to vec1 at cbegin()\n";
    vec1.insert(vec1.cbegin(), -1);
    std::cout << "Insert -2 to vec1 at cbegin() + 1\n";
    vec1.insert(vec1.cbegin() + 1, -2);
    std::for_each(std::begin(vec1), std::end(vec1),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "Size: " << vec1.size() << "\n";
    std::cout << "Capacity: " << vec1.capacity() << "\n";

    vec1.shrink_to_fit();
    std::cout << "Shrink vec1 to fit size: " << vec1.size() << "\n";
    std::cout << "New capacity: " << vec1.capacity() << "\n";
    std::cout << "Insert -3 to vec1 at cbegin() + 2\n";
    vec1.insert(vec1.cbegin() + 2, -3);
    std::cout << "Insert -4 to vec1 at cbegin() + 3\n";
    vec1.insert(vec1.cbegin() + 3, -4);
    vec1.insert(vec1.cend(), 9);
    std::cout << "Insert 9 to vec1 at cend()\n";
    vec1.insert(vec1.cend(), 10);
    std::cout << "Insert 10 to vec1 at cend()\n";
    std::for_each(std::begin(vec1), std::end(vec1),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "Size: " << vec1.size() << "\n";
    std::cout << "Capacity: " << vec1.capacity() << "\n";

    opendsa::vector<int> vec2 = std::move(vec1);
    std::cout << "Transfer ownership from vec1 to vec2\n";
    std::cout << "vec1's size: " << vec1.size() << "\n";
    std::cout << "vec1's capacity: " << vec1.capacity() << "\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

    vec2.insert(vec2.cbegin() + 4, {-5, -6, -7, -8});
    std::cout << "Insert {-5, -6, -7, -8} to vec2 at cbegin() + 4\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

    vec2.insert(vec2.cbegin() + 8, {-9, -10, -11, -12});
    std::cout << "Insert {-9, -10, -11, -12} to vec2 at cbegin() + 8\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

    vec2.insert(vec2.cend() - 2, {100, 200, 300});
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

    opendsa::vector<int> v = {1, 2, 3};
    vec2.insert(vec2.cend(), v.cbegin(), v.cend());
    std::cout << "Insert std::vector{ 1, 2, 3 } to vec2 at cend()\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

    vec2.insert(vec2.cbegin(), 4, 0);
    std::cout << "Insert 0 four times to vec2 at cbegin()\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";
    vec2.insert(vec2.cend() - 1, 3, 500);
    std::cout << "Insert 500 three times to vec2 at cend() - 1\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

    vec2.insert(vec2.cend(), 3, 1000);
    std::cout << "Insert 1000 three times to vec2 at cend()\n";
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";

		opendsa::vector<int> vec4(10);
    std::for_each(std::begin(vec4), std::end(vec4),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec4 size: " << vec4.size() << "\n";
    std::cout << "vec4 capacity: " << vec4.capacity() << "\n";

		opendsa::vector<int> vec5(vec2);
    std::for_each(std::begin(vec5), std::end(vec5),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec5 size: " << vec5.size() << "\n";
    std::cout << "vec5 capacity: " << vec5.capacity() << "\n";
		
		vec2.insert(vec2.cend(), 3, 200);
    std::for_each(std::begin(vec2), std::end(vec2),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec2 size: " << vec2.size() << "\n";
    std::cout << "vec2 capacity: " << vec2.capacity() << "\n";
    std::for_each(std::begin(vec5), std::end(vec5),
                  [](const int &e) { std::cout << e << " "; });
    std::cout << "\n";
    std::cout << "vec5 size: " << vec5.size() << "\n";
    std::cout << "vec5 capacity: " << vec5.capacity() << "\n";

    return 0;
}
