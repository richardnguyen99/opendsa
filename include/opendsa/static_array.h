/**
 * @file container.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Headers for list-based container
 * @version 0.1
 * @date 2021-06-26
 *
 * @copyright Copyright (c) 2021
 */

#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#pragma once

namespace opendsa
{

    /**
     * @brief An experimental container for storing a fixed-size sequence of
     * elements.
     *
     * @tparam T Type of elements
     * @tparam Size Number of elements
     */
    template <typename T>
    class static_array
    {
    public:
        template <class _T, class _U = std::remove_cv_t<_T>>
        class _internal_iterator
            : public std::iterator<std::forward_iterator_tag, _U,
                                   std::ptrdiff_t, _T *, _T &>
        {
            _U *ptr_;

        public:
            explicit constexpr _internal_iterator(_U *ptr) : ptr_(ptr) {}
            constexpr _internal_iterator() : ptr_(nullptr) {}

            // operations
            constexpr void swap(_internal_iterator &other) noexcept
            {
                std::swap(ptr_, other.ptr_);
            }

            // iterators
            constexpr _internal_iterator &operator++()
            {
                ptr_++;
                return *this;
            }

            constexpr _internal_iterator operator++(int)
            {
                _internal_iterator tmp{ptr_};
                ptr_++;
                return tmp;
            }

            constexpr _internal_iterator &operator--()
            {
                ptr_--;
                return *this;
            }

            constexpr _internal_iterator &operator--() const
            {
                ptr_--;
                return *this;
            }

            constexpr _internal_iterator operator--(int)
            {
                _internal_iterator tmp{ptr_};
                ptr_--;
                return tmp;
            }

            constexpr _internal_iterator operator--(int) const
            {
                _internal_iterator tmp{ptr_};
                ptr_--;
                return tmp;
            }

            constexpr bool operator==(_internal_iterator &other) noexcept
            {
                return ptr_ == other.ptr_;
            }

            constexpr bool
            operator==(const _internal_iterator &other) const noexcept
            {
                return ptr_ == other.ptr_;
            }

            constexpr bool operator!=(_internal_iterator &other) noexcept
            {
                return ptr_ != other.ptr_;
            }

            constexpr bool
            operator!=(const _internal_iterator &other) const noexcept
            {
                return ptr_ != other.ptr_;
            }

            constexpr _T &operator*() const { return *ptr_; }
            constexpr _T &operator->() const { return ptr_; }

            constexpr operator _internal_iterator<const _T>() const
            {
                return _internal_iterator<const _T>(ptr_);
            }
        };

        using value_type             = T;
        using size_type              = std::size_t;
        using pointer                = T *;
        using reference              = T &;
        using const_pointer          = const T *;
        using const_reference        = const T &;
        using difference_type        = std::ptrdiff_t;
        using iterator               = _internal_iterator<value_type>;
        using const_iterator         = _internal_iterator<const value_type>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // Member function
        constexpr explicit static_array() noexcept
            : size_{8}, container_{std::make_unique<value_type[]>(size_)}
        {
        }
        constexpr static_array(size_type count)
            : size_{count}, container_{std::make_unique<value_type[]>(size_)}
        {
        }

        template <typename Iter>
        constexpr static_array(Iter first, Iter last) noexcept
        {
            size_      = std::distance(first, last);
            container_ = std::make_unique<T[]>(size_);

            for (Iter cursor = first; cursor != last; cursor++)
            {
                size_type idx = std::distance(first, cursor);

                container_[idx] = static_cast<value_type>(*cursor);
            }
        }

        constexpr static_array(const static_array &other) noexcept
        {
            size_      = std::distance(other.cbegin(), other.cend());
            container_ = std::make_unique<T[]>(size_);

            for (auto cursor = other.cbegin(); cursor != other.cend(); cursor++)
            {
                size_type idx = std::distance(other.cbegin(), cursor);

                container_[idx] = *cursor;
            }
        }

        constexpr static_array(static_array &&other) noexcept
        {
            size_      = std::distance(std::begin(other), std::end(other));
            container_ = std::move(other.container_);
        }

        constexpr static_array(std::initializer_list<value_type> init)
        {
            size_      = init.size();
            container_ = std::make_unique<T[]>(size_);

            for (auto cursor = init.begin(); cursor != init.end(); ++cursor)
            {
                size_type idx   = std::distance(init.begin(), cursor);
                container_[idx] = *cursor;
            }
        }

        // Capacity functions
        constexpr size_type size() const noexcept { return size_; }

        constexpr bool empty() const noexcept { return size_ == 0; }

        // Iterators
        constexpr iterator begin() noexcept
        {
            return iterator{container_.get()};
        }

        constexpr const_iterator cbegin() const noexcept
        {
            return const_iterator{container_.get()};
        }

        constexpr iterator end() noexcept
        {
            return iterator{container_.get() + size_};
        }

        constexpr const_iterator cend() const noexcept
        {
            return const_iterator{container_.get() + size_};
        }

        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator{iterator{container_.get() + size_}};
        }

        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator{
                const_iterator{container_.get() + size_}};
        }

        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator{iterator{container_.get()}};
        }

        constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator{const_iterator{container_.get()}};
        }

        // Element access functions
        template <size_type Index>
        constexpr reference get() noexcept
        {
            return container_[Index];
        }

        template <size_type Index>
        constexpr const_reference get() const noexcept
        {
            return container_[Index];
        }

        constexpr reference operator[](size_type index) noexcept
        {
            return container_[index];
        }

        constexpr const_reference operator[](size_type index) const noexcept
        {
            return container_[index];
        }

        constexpr reference at(size_type index)
        {
            if (index >= 0 && index < size_)
                return container_[index];

            throw std::out_of_range("index out of range");
        }

        constexpr const_reference at(size_type index) const
        {
            if (index >= 0 && index < size_)
                return container_[index];

            throw std::out_of_range("index out of range");
        }

        constexpr reference front() noexcept { return container_[0]; }

        constexpr const_reference front() const noexcept
        {
            return container_[0];
        }

        constexpr reference back() noexcept { return container_[size_ - 1]; }

        constexpr const_reference back() const noexcept
        {
            return container_[size_ - 1];
        }

        constexpr pointer data() noexcept { return container_.get(); }

        constexpr const_pointer data() const noexcept
        {
            return container_.get();
        }

        // Operations
        constexpr void fill(const T &value)
        {
            for (int i = 0; i < size_; ++i)
            {
                container_[i] = value;
            }
        }

        constexpr void swap(static_array &other) noexcept
        {
            container_.swap(other.container_);
        }

        friend constexpr bool operator==(const static_array<T> &lhs,
                                         const static_array<T> &rhs) noexcept
        {
            return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
        }

        friend constexpr bool operator!=(const static_array<T> &lhs,
                                         const static_array<T> &rhs) noexcept
        {
            return !(lhs == rhs);
        }

    private:
        std::size_t          size_;
        std::unique_ptr<T[]> container_;
    };

} // namespace opendsa

namespace std
{
    template <std::size_t index, class T>
    constexpr T &get(opendsa::static_array<T> &arr) noexcept
    {
        return arr[index];
    }
} // namespace std
