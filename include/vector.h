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

#include <algorithm>
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

        using iterator               = normal_iterator<pointer, vector>;
        using const_iterator         = normal_iterator<const_pointer, vector>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        vector() : _start(), _finish(), _end() {}

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

            const difference_type n = std::distance(other._start, other._end);
            _start                  = traits_t::allocate(_alloc, n);
            _finish                 = _start;

            for (auto curr = other._start; curr != other._finish; curr++)
            {
                traits_t::construct(_alloc, std::addressof(*_finish), *curr);
                _finish++;
            }

            _end = _start + n;
        }

        constexpr vector(vector &&other)
        {
            this->_start  = other._start;
            this->_finish = other._finish;
            this->_end    = other._end;

            other._start  = pointer();
            other._finish = pointer();
            other._end    = pointer();
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

            for (auto curr = _start; curr != _finish; curr++)
                traits_t::destroy(_alloc, std::addressof(*curr));

            _finish = _start;
            traits_t::deallocate(_alloc, _start, n);
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

        constexpr const_iterator cbegin() const noexcept
        {
            return const_iterator(_start);
        }

        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }

        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        constexpr iterator end() noexcept { return iterator(_finish); }

        constexpr const_iterator cend() const noexcept
        {
            return const_iterator(_finish);
        }

        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }

        constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

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

        constexpr void reserve(size_type new_cap)
        {
            if (this->capacity() < new_cap)
            {
                using traits_t           = std::allocator_traits<allocator>;
                const size_type old_size = size();

                pointer new_start = traits_t::allocate(_alloc, new_cap);
                for (size_type i = 0; i < new_cap; i++)
                    traits_t::construct(_alloc,
                                        std::addressof(*(new_start + i)),
                                        *(_start + i));

                for (pointer curr = _start; curr != _finish; curr++)
                    traits_t::destroy(_alloc, std::addressof(*curr));

                traits_t::deallocate(_alloc, _start, _end - _start);

                _start  = new_start;
                _finish = new_start + old_size;
                _end    = _start + new_cap;
            }
        }

        constexpr void shrink_to_fit()
        {
            if (this->capacity() > this->size())
            {
                using traits_t          = std::allocator_traits<allocator>;
                const size_type new_cap = this->size();

                pointer new_start = traits_t::allocate(_alloc, new_cap);
                for (size_type i = 0; i < new_cap; i++)
                {
                    traits_t::construct(_alloc,
                                        std::addressof(*(new_start + i)),
                                        *(_start + i));
                }

                for (pointer curr = _start; curr != _finish; curr++)
                    traits_t::destroy(_alloc, std::addressof(*curr));

                traits_t::deallocate(_alloc, _start, _end - _start);

                _start  = new_start;
                _finish = new_start + new_cap;
                _end    = _start + new_cap;
            }
        }

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
            }
        }

        constexpr iterator insert(const_iterator pos, const _Tp &value)
        {
            using traits_t          = std::allocator_traits<allocator>;
            const size_type n       = pos - begin();
            const auto      new_pos = begin() + (pos - cbegin());

            if (_finish != _end)
            {
                if (pos == cend())
                {
                    traits_t::construct(_alloc, _finish, value);
                    ++_finish;
                }
                else
                    _insert_helper(new_pos, value);
            }
            else
                _insert_realloc(new_pos, value);

            return iterator(_start + n);
        }

        constexpr iterator insert(const_iterator pos, _Tp &&value)
        {
            using traits_t          = std::allocator_traits<allocator>;
            const size_type n       = pos - begin();
            const auto      new_pos = begin() + (pos - cbegin());

            if (_finish != _end)
            {
                if (pos == cend())
                {
                    traits_t::construct(_alloc, _finish, std::move(value));
                    ++_finish;
                }
                else
                    _insert_helper(new_pos, std::move(value));
            }
            else
                _insert_realloc(new_pos, std::move(value));

            return iterator(_start + n);
        }

        const iterator insert(const_iterator pos, size_type n,
                              const value_type &value)
        {
            difference_type offset = pos - cbegin();

            _insert_fill(begin() + offset, n, value);

            return begin() + offset;
        }

        template <typename _InputIter,
                  typename = std::_RequireInputIter<_InputIter>>
        constexpr iterator insert(const_iterator pos, _InputIter first,
                                  _InputIter last)
        {
            difference_type offset = pos - cbegin();

            _insert_range(begin() + offset, first, last,
                          std::forward_iterator_tag());

            return begin() + offset;
        }

        constexpr iterator insert(const_iterator                    pos,
                                  std::initializer_list<value_type> list)
        {
            difference_type offset = pos - cbegin();

            _insert_range(begin() + offset, list.begin(), list.end(),
                          std::forward_iterator_tag());

            return begin() + offset;
        }

        template <typename... Args>
        constexpr iterator emplace(const_iterator pos, Args &&...args)
        {
            using traits_t          = std::allocator_traits<allocator>;
            const size_type n       = pos - begin();
            const auto      new_pos = begin() + (pos - cbegin());

            if (_finish != _end)
            {
                if (pos == cend())
                {
                    traits_t::construct(_alloc, _finish,
                                        std::forward<Args>(args)...);
                    ++_finish;
                }
                else
                    _insert_helper(new_pos, std::forward<Args>(args)...);
            }
            else
                _insert_realloc(new_pos, std::forward<Args>(args)...);

            return iterator(_start + n);
        }

        template <typename... Args>
        constexpr iterator emplace_back(Args &&...args)
        {
            return emplace(cend(), std::forward<Args>(args)...);
        }

        constexpr void push_back(const value_type &value)
        {
            using traits_t = std::allocator_traits<allocator>;
            if (_finish != _end)
            {
                traits_t::construct(_alloc, _finish, value);
                ++_finish;
            }
            else
                _insert_realloc(end(), value);
        }

        constexpr void push_back(_Tp &&value)
        {
            emplace_back(std::move(value));
        }

        constexpr iterator erase(const_iterator pos)
        {
            using traits_t = std::allocator_traits<allocator>;

            iterator normal_pos = begin() + (pos - cbegin());

            if (normal_pos + 1 != end())
                std::move(normal_pos + 1, end(), normal_pos);

            _finish--;

            return normal_pos;
        }

        constexpr iterator erase(const_iterator first, const_iterator last)
        {
            using traits_t = std::allocator_traits<allocator>;

            iterator normal_first = begin() + (first - cbegin());
            iterator normal_last  = begin() + (last - cbegin());

            if (normal_first != normal_last)
            {
                if (normal_last != end())
                    std::move(normal_last, end(), normal_first);

                pointer erase_start     = normal_first.base() + (cend() - last);
                const difference_type n = std::distance(erase_start, _finish);
                if (n)
                {
                    for (auto curr = erase_start; curr != _finish; curr++)
                        traits_t::destroy(_alloc, std::addressof(*curr));

                    _finish = erase_start;
                    //traits_t::deallocate(_alloc, erase_start, n);
                }
            }

            return normal_first;
        }

    private:
        allocator _alloc;
        pointer   _start;
        pointer   _finish;
        pointer   _end;

        size_type _check_len(size_type n, const char *s) const
        {
            if (max_size() - size() < n)
                std::__throw_length_error("New size exceeds max_size()");

            const size_type len = size() + std::max(size(), n);

            return (len < size() || len > max_size()) ? max_size() : len;
        }

        template <typename Arg>
        void _insert_helper(iterator pos, Arg &&arg)
        {
            using traits_t     = std::allocator_traits<allocator>;
            const auto new_pos = begin() + (pos - cbegin());

            traits_t::construct(_alloc, std::addressof(*_finish),
                                std::move(*(_finish - 1)));
            ++_finish;

            pointer first  = new_pos.base();
            pointer last   = _finish - 2;
            pointer d_last = _finish - 1;

            while (first != last)
                *(--d_last) = std::move(*(--last));

            *new_pos = std::forward<Arg>(arg);
        }

        template <typename... _Arg>
        void _insert_realloc(iterator pos, _Arg &&...arg)
        {
            using traits_t             = std::allocator_traits<allocator>;
            const size_type len        = _check_len(1, "vector::insert");
            pointer         old_start  = this->_start;
            pointer         old_finish = this->_finish;
            pointer         new_start  = traits_t::allocate(_alloc, len);
            pointer         new_finish = pointer();
            const size_type n          = pos - begin();

            try
            {
                traits_t::construct(_alloc, new_start + n,
                                    std::forward<_Arg>(arg)...);
                new_finish
                    = std::uninitialized_move(old_start, pos.base(), new_start);
                ++new_finish;
                new_finish = std::uninitialized_move(pos.base(), old_finish,
                                                     new_finish);
            }
            catch (...)
            {
                if (!new_finish)
                    traits_t::destroy(_alloc, new_start + n);
                else
                    for (auto curr = new_start; curr != new_finish; curr++)
                        traits_t::destroy(_alloc, std::addressof(*curr));
                traits_t::deallocate(_alloc, new_start, len);
            }

            for (pointer curr = old_start; curr != old_finish; curr++)
                traits_t::destroy(_alloc, std::addressof(*curr));

            traits_t::deallocate(_alloc, old_start, _end - old_start);

            this->_start  = new_start;
            this->_finish = new_finish;
            this->_end    = new_start + len;
        }

        void _insert_fill(iterator pos, size_type n, const value_type &value)
        {
            if (n == 0)
                return;

            if (size_type(_end - _finish) >= n)
            {
                const size_type elems_after_pos = this->end() - pos;
                pointer         old_finish(_finish);
                _Tp             copy(value);

                if (elems_after_pos > n)
                {
                    std::uninitialized_move(_finish - n, _finish, _finish);
                    _finish += n;

                    pointer o_first = pos.base();
                    pointer o_last  = old_finish - n;
                    pointer d_last  = old_finish;

                    while (o_first != o_last)
                        *(--d_last) = std::move(*(--o_last));

                    std::fill(pos.base(), pos.base() + n, copy);
                }
                else
                {
                    _finish = std::uninitialized_fill_n(
                        _finish, n - elems_after_pos, copy);
                    std::uninitialized_move(pos.base(), old_finish, _finish);
                    _finish += elems_after_pos;
                    std::fill(pos.base(), old_finish, copy);
                }
            }
            else
            {
                using traits_t      = std::allocator_traits<allocator>;
                const size_type len = _check_len(n, "vector::_insert_range");
                pointer         new_start  = traits_t::allocate(_alloc, len);
                pointer         new_finish = new_start;

                try
                {
                    new_finish = std::uninitialized_move(this->_start,
                                                         pos.base(), new_start);
                    std::uninitialized_fill_n(new_finish, n, value);
                    new_finish += n;
                    new_finish = std::uninitialized_move(pos.base(), _finish,
                                                         new_finish);
                }
                catch (...)
                {
                    for (pointer curr = new_start; curr != new_finish; curr++)
                        traits_t::destroy(_alloc, std::addressof(*curr));

                    traits_t::deallocate(_alloc, new_start, len);
                }

                for (pointer curr = _start; curr != _finish; curr++)
                    traits_t::destroy(_alloc, std::addressof(*curr));

                traits_t::deallocate(_alloc, _start, _end - _start);

                this->_start  = new_start;
                this->_finish = new_finish;
                this->_end    = new_start + len;
            }
        }

        template <typename _ForwardIterator>
        void _insert_range(iterator pos, _ForwardIterator first,
                           _ForwardIterator last, std::forward_iterator_tag)
        {
            if (first == last)
                return;

            const size_type n = std::distance(first, last);
            if (size_type(_end - _finish) >= n)
            {
                const size_type elems_after_pos = this->end() - pos;
                pointer         old_finish(_finish);

                if (elems_after_pos > n)
                {
                    std::uninitialized_move(_finish - n, _finish, _finish);
                    _finish += n;

                    pointer o_first = pos.base();
                    pointer o_last  = old_finish - n;
                    pointer d_last  = old_finish;

                    while (o_first != o_last)
                        *(--d_last) = std::move(*(--o_last));

                    std::copy(first, last, pos);
                }
                else
                {
                    _ForwardIterator mid = first;
                    std::advance(mid, elems_after_pos);
                    std::uninitialized_copy(mid, last, _finish);
                    _finish += n - elems_after_pos;
                    std::uninitialized_move(pos.base(), old_finish, _finish);
                    _finish += elems_after_pos;
                    std::copy(first, mid, pos);
                }
            }
            else
            {
                using traits_t      = std::allocator_traits<allocator>;
                const size_type len = _check_len(n, "vector::_insert_range");
                pointer         new_start  = traits_t::allocate(_alloc, len);
                pointer         new_finish = new_start;

                try
                {
                    new_finish = std::uninitialized_move(this->_start,
                                                         pos.base(), new_start);
                    new_finish
                        = std::uninitialized_copy(first, last, new_finish);
                    new_finish = std::uninitialized_move(pos.base(), _finish,
                                                         new_finish);
                }
                catch (...)
                {
                    for (pointer curr = new_start; curr != new_finish; curr++)
                        traits_t::destroy(_alloc, std::addressof(*curr));

                    traits_t::deallocate(_alloc, new_start, len);
                }

                for (pointer curr = _start; curr != _finish; curr++)
                    traits_t::destroy(_alloc, std::addressof(*curr));

                traits_t::deallocate(_alloc, _start, _end - _start);

                this->_start  = new_start;
                this->_finish = new_finish;
                this->_end    = new_start + len;
            }
        }
    };
} // namespace opendsa

#endif
