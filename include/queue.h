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
    requires FIFOSequenceContainer<_Sequence,
                                   _Tp> && SwappableContainer<_Sequence, _Tp>
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

        // Capacity

        /**
         * @brief Returns true if the %queue is empty.
         */
        bool empty() const
        {
            return _cont.empty();
        }

        /**
         * @brief  Returns number of elements in the %queue.
         */
        size_type size() const
        {
            return _cont.size();
        }

        // Modifier

        /**
         * @brief Add new data to the end of the %queue.
         *
         * @param x  New data to be pushed.
         *
         * This typical queue operation will depend on what the underlying.
         * container is.
         */
        void push(const value_type &x)
        {
            _cont.push_back(x);
        }

        /**
         * @brief Add an rvalue data to the end of the %queue.
         *
         * @param x Rvalue data to be pushed.
         *
         * This typical queue operation will depend on what the underlying.
         * container is.
         */
        void push(value_type &&x)
        {
            _cont.push_back(std::move(x));
        }

        /**
         * @brief Constructs a new data at the end of the %queue.
         *
         * @param args Argument list to create a new data of type _Tp.
         *
         * This typical queue operation will depend on what the underlying
         * container is.
         */
        template <typename... Args>
        decltype(auto) emplace(Args &&...args)
        {
            return _cont.emplace_back(std::forward<Args>(args)...);
        }

        /**
         * @brief Removes the first item on the %queue.
         *
         * This typical queue operation will shrink the size by one. The time
         * complexity will depend on how pop() is implemented in the underlying
         * container. Also, it doesn't return anything if data is needed(), use
         * front() before popping.
         */
        void pop()
        {
            _cont.pop_front();
        }

        /**
         * @brief Swaps the content between two queues.
         *
         * @param q Other existing queue.
         *
         * The time complexity of this function will depend on the swapping
         * technique of the underlying container.
         */
        void swap(queue &q)
        {
            _cont.swap(q._cont);
        }

    private:
        _Sequence _cont;
    };
} // namespace opendsa

#endif /* __OPENDSA_QUEUE_H */
