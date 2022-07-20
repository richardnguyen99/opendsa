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

#include "algorithm.h"
#include "deque.h"

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

        /**
         * @brief Constructs a queue with no elements.
         */
        queue() : _cont() {}

        /**
         * @brief Constructs a queue by copying the container of same type
         * _Sequence.
         *
         * @param c Another container of type _Sequence.
         */
        explicit queue(const container_type &c) : _cont(c) {}

        /**
         * @brief Constructs a queue by moving the contianer of same type
         * _Sequence.
         *
         * @param c Another container of type _Sequence.
         */
        explicit queue(container_type &&c) : _cont(std::move(c)) {}

        /**
         * @brief Constructs a queue by copying an existing queue.
         *
         * @param q Another existing queue of the same.
         */
        queue(const queue &q) : _cont(q._cont) {}

        /**
         * @brief Constructs a queue by moving an existing queue.
         *
         * @param q  Another existing queue of the same
         */
        queue(queue &&q) : _cont(std::move(q._cont)) {}

        // Elements access

        /**
         * @brief Returns a read/write reference to the first item of the
         * %queue.
         */
        reference front()
        {
            return _cont.front();
        }

        /**
         * @brief Returns a readonly reference to the first item of the %queue.
         */
        const_reference front() const
        {
            return _cont.front();
        }

        /**
         * @brief Returns a read/write reference to the last item of the %queue.
         */
        reference back()
        {
            return _cont.back();
        }

        /**
         * @brief Returns a readonly reference to the last item of the %queue.
         */
        const_reference back() const
        {
            return _cont.back();
        }

    private:
        _Sequence _cont;
    };
} // namespace opendsa

#endif /* __OPENDSA_QUEUE_H */
