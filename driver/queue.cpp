#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <vector>

#include "deque.h"
#include "queue.h"

int main(int argc, const char **argv)
{
    opendsa::deque<int>                      d = {1, 2, 3, 4, 5, 6, 7, 8};
    opendsa::queue<int, opendsa::deque<int>> q;
    opendsa::queue<int, opendsa::deque<int>> q1(d);
    opendsa::queue<int, opendsa::deque<int>> q2(q1);
    opendsa::queue<int>                      q3(d);
    opendsa::queue<int>                      q4 = std::move(q3);

    // Uncomment the next section to see the error with wrong-type declarations
    // opendsa::queue<int, opendsa::deque<float>> q5    // wrong type
    // opendsa::queue<int< opendsa::vector<int>> q6     // no FIFO support
}
