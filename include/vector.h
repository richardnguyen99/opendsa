/**
 * @file vector.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief A C++ class stores contigious elements
 * @version 0.2
 * @date 2022-06-13
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_VECTOR_H
#define __OPENDSA_VECTOR_H 1

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <sstream>

#include "iterator.h"

namespace opendsa
{
    template <typename _Tp>
    class vector
    {
    public:
        using allocator = std::allocator<_Tp>;
        using pointer   = typename std::allocator_traits<allocator>::pointer;
        using const_pointer =
            typename std::allocator_traits<allocator>::const_pointer;
        using reference       = _Tp &;
        using value_type      = _Tp;
        using const_reference = const _Tp &;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using iterator       = normal_iterator<pointer, vector>;
        using const_iterator = normal_iterator<const_pointer, vector>;

        vector() : _alloc(), _start(), _finish(), _end() {}

        constexpr explicit vector(size_type n)
        {
            using traits_t = std::allocator_traits<allocator>;

            _start  = traits_t::allocate(_alloc, n);
            _finish = std::__uninitialized_default_n_a(_start, n, _alloc);
            _end    = _start + n;
        }

        constexpr vector(size_type n, const _Tp &value)
        {
            using traits_t = std::allocator_traits<allocator>;

            _start  = traits_t::allocate(_alloc, n);
            _finish = std::__uninitialized_fill_n_a(_start, n, value, _alloc);
            _end    = _start + n;
        }

        template <typename _InputIter,
                  typename = std::_RequireInputIter<_InputIter>>
        constexpr vector(_InputIter first, _InputIter last)
        {
            using traits_t = std::allocator_traits<allocator>;

            const difference_type n = std::distance(first, last);
            _start                  = traits_t::allocate(_alloc, n);
            _finish                 = _start;

            for (auto curr = first; curr != last; curr++)
            {
                traits_t::construct(_alloc, std::addressof(*_finish), *curr);
                _finish++;
            }

            _end = _start + n;
        }

        constexpr vector(const vector &other)
        {
            using traits_t = std::allocator_traits<allocator>;

            const difference_type n
                = std::distance(other._start, other._finish);
            _start  = traits_t::allocate(_alloc, n);
            _finish = _start;

            for (auto curr = other._start; curr != other._finish; curr++)
            {
                traits_t::construct(_alloc, std::addressof(*_finish), *curr);
                _finish++;
            }

            _end = _start + n;
        }

        constexpr vector(vector &&other)
        {
            using traits_t = std::allocator_traits<allocator>;

            const difference_type n
                = std::distance(other._start, other._finish);
            _start  = traits_t::allocate(_alloc, n);
            _finish = _start;

            for (auto curr = other._start; curr != other._finish; curr++)
            {
                traits_t::construct(_alloc, std::addressof(*_finish),
                                    std::move(*curr));
                _finish++;
            }

            other.clear();

            _end = _start + n;
        }

        constexpr vector(std::initializer_list<_Tp> init)
        {
            using traits_t = std::allocator_traits<allocator>;

            const difference_type n = std::distance(init.begin(), init.end());
            _start                  = traits_t::allocate(_alloc, n);
            _finish                 = _start;
            for (auto curr = init.begin(); curr != init.end(); curr++)
            {
                traits_t::construct(_alloc, std::addressof(*_finish), *curr);
                _finish++;
            }
            _end = _start + n;
        }

        ~vector()
        {
            using traits_t          = std::allocator_traits<allocator>;
            const difference_type n = std::distance(_start, _finish);

            if (n)
            {
                for (auto curr = _start; curr != _finish; curr++)
                    traits_t::destroy(_alloc, std::addressof(*curr));

                _finish = _start;
                traits_t::deallocate(_alloc, _start, n);
            }
        }

        // Access
        constexpr reference at(size_type pos)
        {
            const difference_type n = std::distance(_start, _finish);

            if (pos >= n)
            {
                std::ostringstream msg;
                msg << "pos (which is " << pos << ") is out of bound (which is "
                    << n << ").";
                throw std::out_of_range(msg.str());
            }

            return *(_start + pos);
        }

        constexpr const_reference at(size_type pos) const
        {
            const difference_type n = std::distance(_start, _finish);

            if (pos >= n)
            {
                std::ostringstream msg;
                msg << "pos (which is " << pos << ") is out of bound (which is "
                    << n << ").";
                throw std::out_of_range(msg.str());
            }

            return *(_start + pos);
        }

        constexpr reference operator[](size_type pos)
        {
            return *(_start + pos);
        }

        constexpr const_reference operator[](size_type pos) const
        {
            return *(_start + pos);
        }

        constexpr reference front() { return *(_start); }

        constexpr const_reference front() const { return *(_start); }

        constexpr reference back() { return *(_finish - 1); }

        constexpr const_reference back() const { return *(_finish - 1); }

        constexpr _Tp *data() noexcept { return _start; }

        constexpr const _Tp *data() const noexcept { return _start; }

        // Iterator

        constexpr iterator begin() noexcept { return iterator(_start); }

        constexpr iterator end() noexcept { return iterator(_finish); }

        // Capacity
        constexpr bool empty() const noexcept { return (_start == _finish); }

        const size_type max_size() const noexcept
        {
            using traits_t = std::allocator_traits<allocator>;

            return traits_t::max_size(_alloc);
        }

        constexpr size_type size() const noexcept
        {
            return size_type(_finish - _start);
        }

        constexpr void reserve(size_type new_cap) {}

        constexpr size_type capacity() const noexcept
        {
            return size_type(_end - _start);
        }

        // Modifiers
        constexpr void clear() noexcept
        {
            using traits_t = std::allocator_traits<allocator>;

            const difference_type n = std::distance(_start, _finish);
            if (n)
            {
                for (auto curr = _start; curr != _finish; curr++)
                    traits_t::destroy(_alloc, std::addressof(*curr));

                _finish = _start;
                traits_t::deallocate(_alloc, _start, n);
            }
        }

    private:
        allocator _alloc;
        pointer   _start;
        pointer   _finish;
        pointer   _end;
    };
} // namespace opendsa

#endif
