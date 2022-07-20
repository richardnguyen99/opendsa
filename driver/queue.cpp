#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <vector>

#include "deque.h"
#include "queue.h"

template <typename T>
void test_get_queue_info(const opendsa::queue<T> &q, const char *qname)
{
    std::cout << "==========" << qname << "==========\n\n";
    std::cout << "Empty?: " << (q.empty() ? "Yes" : "No") << "\n";
    std::cout << "Size: " << q.size() << "\n";
    std::cout << "Front: " << q.front() << "\n";
    std::cout << "Back: " << q.back() << "\n";
    std::cout << "\n";
}

int main(int argc, const char **argv)
{
    opendsa::deque<int>                      d = {1, 2, 3, 4, 5, 6, 7, 8};
    opendsa::queue<int, opendsa::deque<int>> q;
    opendsa::queue<int, opendsa::deque<int>> q1(d);
    opendsa::queue<int, opendsa::deque<int>> q2(q1);
    opendsa::queue<int>                      q3(d);
    opendsa::queue<int>                      q4(std::move(q3));
    q4.swap(q3);

    // Uncomment the next section to see the error with wrong-type declarations
    // opendsa::queue<int, opendsa::deque<float>> q5    // wrong type
    // opendsa::queue<int< opendsa::vector<int>> q6     // no FIFO support
    test_get_queue_info(q3, "Queue 3");
    test_get_queue_info(q4, "Queue 4");
}
