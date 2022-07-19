/**
 * @file queue.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Class template that gives programmers the functionality of a queue - A
 * first-in, first-out data structure
 * @version 0.1
 * @date 2022-07-18
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_QUEUE_H
#define __OPENDSA_QUEUE_H 1

#include <opendsa/algorithm>
#include <opendsa/deque>

namespace opendsa
{
    template <typename _Tp, typename _Sequence = opendsa::deque<_Tp>>
    requires FIFOSequenceContainer<_Sequence, _Tp>
    class queue
    {
    public:
        using value_type      = _Sequence::value_type;
        using reference       = _Sequence::reference;
        using const_reference = _Sequence::const_reference;
        using size_type       = _Sequence::size_type;
        using container_type  = _Sequence;

        queue() : c() {}

    private:
        _Sequence c;
    };
} // namespace opendsa

#endif /* __OPENDSA_QUEUE_H */
