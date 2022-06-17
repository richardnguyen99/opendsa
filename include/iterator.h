/**
 * @file iterator.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_ITERATOR_H
#define __OPENDSA_ITERATOR_H 1

#include <iterator>

namespace opendsa
{
    template <typename _Pointer, typename _Container>
    class normal_iterator
    {
    protected:
        _Pointer _current;

        using traits_type = std::iterator_traits<_Pointer>;

    public:
        using iterator_type     = _Pointer;
        using iterator_category = typename traits_type::iterator_category;
        using value_type        = typename traits_type::value_type;
        using difference_type   = typename traits_type::difference_type;
        using reference         = typename traits_type::reference;
        using pointer           = typename traits_type::pointer;

        constexpr normal_iterator() noexcept : _current(_Pointer()) {}

        explicit normal_iterator(const _Pointer &_p) noexcept : _current(_p) {}

        template <typename _Iter>
        normal_iterator(
            const normal_iterator<
                _Iter, typename std::enable_if<
                           (std::__are_same<
                               _Iter, typename _Container::pointer>::__value),
                           _Container>::__type> &_i) noexcept
            : _current(_i.base())
        {
        }

        reference operator*() const noexcept { return *_current; }

        pointer operator->() const noexcept { return _current; }

        normal_iterator &operator++() noexcept
        {
            ++_current;
            return *this;
        }

        normal_iterator operator++(int) noexcept
        {
            return normal_iterator(_current++);
        }

        normal_iterator &operator--() noexcept
        {
            --_current;
            return *this;
        }

        normal_iterator operator--(int) noexcept
        {
            return normal_iterator(_current--);
        }

        reference operator[](difference_type pos) const noexcept
        {
            return _current[pos];
        }

        normal_iterator &operator+=(difference_type n) noexcept
        {
            _current += n;
            return *this;
        }

        normal_iterator operator+(difference_type n) const noexcept
        {
            return normal_iterator(_current + n);
        }

        normal_iterator &operator-=(difference_type n) noexcept
        {
            _current -= n;
            return *this;
        }

        normal_iterator operator-(difference_type n) const noexcept
        {
            return normal_iterator(_current - n);
        }

        const _Pointer &base() const noexcept { return _current; }
    };

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator==(const normal_iterator<_IteratorL, _Container> &lhs,
               const normal_iterator<_IteratorR, _Container> &rhs) noexcept
    {
        return lhs.base() == rhs.base();
    }

    template <typename _Iterator, typename _Container>
    inline bool
    operator==(const normal_iterator<_Iterator, _Container> &lhs,
               const normal_iterator<_Iterator, _Container> &rhs) noexcept
    {
        return lhs.base() == rhs.base();
    }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator!=(const normal_iterator<_IteratorL, _Container> &lhs,
               const normal_iterator<_IteratorR, _Container> &rhs) noexcept
    {
        return lhs.base() != rhs.base();
    }

    template <typename _Iterator, typename _Container>
    inline bool
    operator!=(const normal_iterator<_Iterator, _Container> &lhs,
               const normal_iterator<_Iterator, _Container> &rhs) noexcept
    {
        return lhs.base() != rhs.base();
    }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<(const normal_iterator<_IteratorL, _Container> &lhs,
              const normal_iterator<_IteratorR, _Container> &rhs) noexcept
    {
        return lhs.base() < rhs.base();
    }

    template <typename _Iterator, typename _Container>
    inline bool
    operator<(const normal_iterator<_Iterator, _Container> &lhs,
              const normal_iterator<_Iterator, _Container> &rhs) noexcept
    {
        return lhs.base() < rhs.base();
    }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<=(const normal_iterator<_IteratorL, _Container> &lhs,
               const normal_iterator<_IteratorR, _Container> &rhs) noexcept
    {
        return lhs.base() <= rhs.base();
    }

    template <typename _Iterator, typename _Container>
    inline bool
    operator<=(const normal_iterator<_Iterator, _Container> &lhs,
               const normal_iterator<_Iterator, _Container> &rhs) noexcept
    {
        return lhs.base() <= rhs.base();
    }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>(const normal_iterator<_IteratorL, _Container> &lhs,
              const normal_iterator<_IteratorR, _Container> &rhs) noexcept
    {
        return lhs.base() > rhs.base();
    }

    template <typename _Iterator, typename _Container>
    inline bool
    operator>(const normal_iterator<_Iterator, _Container> &lhs,
              const normal_iterator<_Iterator, _Container> &rhs) noexcept
    {
        return lhs.base() > rhs.base();
    }

    template <typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>=(const normal_iterator<_IteratorL, _Container> &lhs,
               const normal_iterator<_IteratorR, _Container> &rhs) noexcept
    {
        return lhs.base() >= rhs.base();
    }

    template <typename _Iterator, typename _Container>
    inline bool
    operator>=(const normal_iterator<_Iterator, _Container> &lhs,
               const normal_iterator<_Iterator, _Container> &rhs) noexcept
    {
        return lhs.base() >= rhs.base();
    }

} // namespace opendsa

#endif
