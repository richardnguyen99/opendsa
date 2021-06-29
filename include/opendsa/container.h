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
        public:
            // Member functions
            explicit _internal_iterator(std::size_t   index,
                                        static_array &container)
                : index_{index}, container_{container}
            {
            }
            _internal_iterator() : index_{0}, container_{nullptr} {}

            // operations
            constexpr void swap(_internal_iterator<T> &other) noexcept
            {
                std::swap(container_[index_], container_[other.index_]);
            }

            // iterators
            constexpr _internal_iterator<T> &operator++()
            {
                index_++;
                return *this;
            }

            constexpr _internal_iterator<T> operator++(int)
            {
                _internal_iterator<T> tmp{index_, container_};
                index_++;
                return tmp;
            }

            constexpr _internal_iterator<T> &operator--()
            {
                index_--;
                return *this;
            }

            constexpr _internal_iterator<T> operator--(int)
            {
                _internal_iterator<T> tmp{index_, container_};
                index_--;
                return tmp;
            }

            constexpr bool operator==(const _internal_iterator<T> &other) const
            {
                return index_ == other.index_;
            }

            constexpr bool operator!=(const _internal_iterator<T> &other) const
            {
                return index_ != other.index_;
            }

            constexpr T &operator*() const { return container_[index_]; }

            constexpr operator _internal_iterator<const T>() const
            {
                return _internal_iterator<const T>(index_, container_);
            }

        private:
            std::size_t      index_;
            static_array<T> &container_;
        };

        using value_type      = T;
        using size_type       = std::size_t;
        using pointer         = T *;
        using reference       = T &;
        using const_pointer   = const T *;
        using const_reference = const T &;
        using difference_type = std::ptrdiff_t;
        using iterator        = _internal_iterator<value_type>;
        using const_iterator  = _internal_iterator<const value_type>;
        using riterator       = std::reverse_iterator<iterator>;
        using const_riterator = std::reverse_iterator<const_iterator>;

        // Member function
        constexpr explicit static_array() noexcept
            : max_size_{8}, container_{
                                std::make_unique<value_type[]>(max_size_)}
        {
        }
        constexpr static_array(size_type count)
            : max_size_{count}, container_{
                                    std::make_unique<value_type[]>(max_size_)}
        {
        }

        template <typename Iter>
        constexpr static_array(Iter first, Iter last) noexcept
        {
            max_size_  = std::distance(first, last);
            container_ = std::make_unique<T[]>(max_size_);

            for (Iter cursor = first; cursor != last && count_ < max_size_;
                 cursor++)
            {
                size_type idx = std::distance(first, cursor);

                container_[idx] = static_cast<value_type>(*cursor);
                count_++;
            }
        }

        constexpr static_array(const static_array &other) noexcept
        {
            max_size_  = std::distance(std::begin(other), std::end(other));
            container_ = std::make_unique<T[]>(max_size_);

            for (auto cursor = std::begin(other);
                 cursor != std::end(other) && count_ < max_size_; cursor++)
            {
                size_type idx = std::distance(std::begin(other), cursor);

                container_[idx] = *cursor;
                count_++;
            }
        }

        constexpr static_array(static_array &&other) noexcept
        {
            max_size_  = std::distance(std::begin(other), std::end(other));
            count_     = max_size_;
            container_ = std::move(other.container_);
        }

        constexpr static_array(std::initializer_list<value_type> init)
        {
            max_size_  = init.size();
            container_ = std::make_unique<T[]>(max_size_);

            for (auto cursor = init.begin(); cursor != init.end(); ++cursor)
            {
                size_type idx   = std::distance(init.begin(), cursor);
                container_[idx] = *cursor;
                count_++;
            }
        }

        // Capacity functions
        constexpr size_type size() const noexcept { return count_; }

        constexpr size_type max_size() const noexcept { return max_size_; }

        constexpr bool empty() const noexcept { return count_ == 0; }

        // Iterators
        iterator begin() noexcept { return iterator{0, *this}; }

        const_iterator cbegin() const noexcept
        {
            return const_iterator{0, *this};
        }

        iterator end() noexcept { return iterator{count_, *this}; }

        const_iterator cend() const noexcept
        {
            return const_iterator{count_, *this};
        }

        riterator rbegin() noexcept
        {
            return riterator{iterator{count_, *this}};
        }

        const_riterator crbegin() const noexcept
        {
            return const_riterator{const_iterator{count_, *this}};
        }

        riterator rend() noexcept { return riterator{iterator{0, *this}}; }

        const_riterator crend() const noexcept
        {
            return const_riterator{const_iterator{0, *this}};
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
            if (index >= 0 && index < count_)
                return container_[index];

            throw std::out_of_range("index out of range");
        }

        constexpr const_reference at(size_type index) const
        {
            if (index >= 0 && index < count_)
                return container_[index];

            throw std::out_of_range("index out of range");
        }

        constexpr reference front() noexcept { return container_[0]; }

        constexpr const_reference front() const noexcept
        {
            return container_[0];
        }

        constexpr reference back() noexcept { return container_[count_ - 1]; }

        constexpr const_reference back() const noexcept
        {
            return container_[count_ - 1];
        }

        constexpr pointer data() noexcept { return container_.get(); }

        constexpr const_pointer data() const noexcept
        {
            return container_.get();
        }

        // Operations
        constexpr void fill(const T &value)
        {
            for (auto &e : container_)
                e = value;
        }

        constexpr void swap(static_array &other) noexcept
        {
            container_.swap(other.container_);
        }

        friend bool operator==(const static_array<T> &lhs,
                               const static_array<T> &rhs)
        {
            return std::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        friend bool operator!=(const static_array<T> &lhs,
                               const static_array<T> &rhs)
        {
            return !(lhs == rhs);
        }

    private:
        std::size_t          max_size_;
        std::size_t          count_ = 0;
        std::unique_ptr<T[]> container_;
    };

} // namespace opendsa