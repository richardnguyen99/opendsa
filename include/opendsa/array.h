/**
 * @file static_array.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Header for static arrays
 * @version 0.1
 * @date 2021-12-11
 *
 * @copyright Copyright (c) 2021
 */

#ifndef _STATIC_ARRAY_H
#define _STATIC_ARRAY_H

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace opendsa
{
    /**
     * @brief `opendsa::array` is like C-style array but more functionalities
     * and features for safe usage
     *
     * @tparam T Type of the elements
     * @tparam N Size of the container
     */
    template <typename T, std::size_t N>
    class array
    {
    public:
        using value_type      = T;
        using reference       = value_type &;
        using pointer         = value_type *;
        using const_reference = const value_type &;
        using const_pointer   = const value_type *;

        using iterator               = value_type *;
        using const_iterator         = const value_type *;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        // Constructors

        /**
         * @brief Default constructor. Create an empty array object
         * with specified size N
         */
        constexpr explicit array()
        {
            this->container_ = std::make_unique<value_type[]>(N);
        }

        /**
         * @brief Create an array that is filled with one value
         *
         * @param value Value of same type as element in the array
         */
        constexpr explicit array(const_reference value)
        {
            this->container_ = std::make_unique<value_type[]>(N);

            std::fill_n(begin(), N, value);
        }

        /**
         * @brief Create an array from the content of the other array
         *
         * @param other Container's content to copy
         */
        constexpr array(const array &other)
        {
            this->container_ = std::make_unique<value_type[]>(N);

            if (N >= other.size())
                std::copy(other.begin(), other.end(), begin());

            else
                std::copy(other.begin(), other.begin() + other.size(), begin());
        }

        /**
         * @brief Create an array by moving content from other array
         *
         * @param other Container's content to move
         */
        constexpr array(array &&other) noexcept
        {
            this->container_ = std::make_unique<value_type[]>(N);

            if (N >= other.size())
                std::move(other.begin(), other.end(), begin());
            else
                std::move(other.begin(), other.begin() + other.size(), begin());

            other.container_.reset();
        }

        /**
         * @brief Create an empty array object with an initializer list
         *
         * @param list List of elements of the same type
         */
        constexpr array(std::initializer_list<value_type> list)
        {
            this->container_ = std::make_unique<value_type[]>(N);

            if (N >= list.size())
                std::copy(list.begin(), list.end(), begin());

            else
                std::copy(list.begin(), list.begin() + list.size(), begin());
        }

        // Iterators
        constexpr iterator begin() noexcept { return this->container_.get(); }

        constexpr const_iterator begin() const noexcept
        {
            return this->container_.get();
        }

        constexpr iterator end() noexcept { return this->container_.get() + N; }

        constexpr const_iterator end() const noexcept
        {
            return this->container_.get() + N;
        }

        constexpr const_iterator cbegin() const noexcept
        {
            return this->container_.get();
        }

        constexpr const_iterator cend() const noexcept
        {
            return this->container_.get() + N;
        }

        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }

        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }

        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        // Capacities
        constexpr size_type size() const noexcept { return N; }

        constexpr bool empty() const noexcept { return size() == 0; }

        // Member access
        constexpr reference operator[](size_type i)
        {
            return *(this->container_.get() + i);
        }

        constexpr const_reference operator[](size_type i) const
        {
            return *(this->container_.get() + i);
        }

        constexpr reference at(size_type i)
        {
            if (i >= N)
                throw std::out_of_range("Index is out of range, which is "
                                        + std::to_string(N));

            return this->operator[](i);
        }

        constexpr const_reference at(size_type i) const
        {
            if (i >= N)
                throw std::out_of_range("Index is out of range, which is "
                                        + std::to_string(N));

            return this->operator[](i);
        }

        constexpr reference front() { return this->operator[](0); }

        constexpr const_reference front() const { return this->operator[](0); }

        constexpr reference back() { return this->operator[](N - 1); }

        constexpr const_reference back() const
        {
            return this->operator[](N - 1);
        }

        constexpr pointer data() noexcept { return this->container_.get(); }

        constexpr const_pointer data() const noexcept
        {
            return this->container_.get();
        }

        /**
         * @brief Release the ownership to other direct array
         */
        constexpr pointer release() noexcept
        {
            return this->container_.release();
        }

        // Operations
        constexpr void fill(const T &value)
        {
            std::fill_n(begin(), begin() + N, value);
        }

        constexpr void swap(array &other) noexcept
        {
            this->container_.swap(other.container_);
        }

    private:
        std::unique_ptr<T[]> container_;
    };

    // Non-member functions
    template <class T, std::size_t N>
    constexpr bool operator==(const array<T, N> &x, const array<T, N> &y)
    {
        for (auto x_start = x.begin(), y_start = y.begin();
             x_start != x.end() && y_start != y.end(); ++x_start, ++y_start)
        {
            if (*x_start != *y_start)
                return false;
        }

        return true;
    }

    template <class T, std::size_t N>
    constexpr bool operator!=(const array<T, N> &x, const array<T, N> &y)
    {
        return !(x == y);
    }

    template <class T, std::size_t N>
    constexpr std::ostream &operator<<(std::ostream &out, const array<T, N> &x)
    {
        for (auto begin = std::cbegin(x); begin != std::cend(x); ++begin)
            std::cout << *begin << " ";
        std::cout << std::endl;
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr T &get(array<T, N> &a) noexcept
    {
        static_assert(I < N, "index out of range");
        return a[I];
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr T &&get(array<T, N> &&a) noexcept
    {
        static_assert(I < N, "index out of range");
        return std::move(a[I]);
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr const T &get(const array<T, N> &a) noexcept
    {
        static_assert(I < N, "index out of range");
        return a[I];
    }

    template <std::size_t I, class T, std::size_t N>
    constexpr const T &&get(const array<T, N> &&a) noexcept
    {
        static_assert(I < N, "index out of range");
        return std::move(a[I]);
    }

}; // namespace opendsa

#endif
