/**
 * @file vector.h
 * @author Richar Nguyen (richard.ng0616@gmail.com)
 * @brief Header for dynamic array
 * @version 0.2
 * @date 2021-04-08
 *
 * @copyright Copyright (c) 2021
 */

#ifndef _DYNAMIC_ARRAY_H
#define _DYNAMIC_ARRAY_H 1

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace opendsa
{
    /**
     * @brief `opendsa::vector` is a container that holds contiguous elements
     * and expands size dynamically
     *
     * @tparam T Type of elements that the container will hold
     */
    template <typename T>
    class vector
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
         * @brief Default constructor. Create a %vector with empty size.
         */
        constexpr vector() : size_(0)
        {
            this->container_ = std::make_unique<value_type[]>(this->capacity_);
        }

        /**
         * @brief Create a %vector by filling %total number of value
         *
         * @param total Number of elements to insert
         * @param value Value of the same type to insert
         */
        explicit constexpr vector(size_type total, const_reference value)
            : size_(total), capacity_(total)
        {
            this->container_ = std::make_unique<value_type[]>(total);

            std::fill(begin(), end(), value);
        }

        /**
         * @brief  Create an empty %vector that holds %total elements
         *
         * @param total Number of elements to insert
         */
        explicit constexpr vector(size_type total) : size_(0), capacity_(total)
        {
            this->container_ = std::make_unique<value_type[]>(total);
        }

        /**
         * @brief Range-based constructor. Create a %vector with the contents
         * from [first, last)
         *
         * @param first Inclusive lower-bound iterator of the range
         * @param last Exclusive upper-bound iterator of the range
         */
        template <class InputIter, typename = std::_RequireInputIter<InputIter>>
        constexpr vector(InputIter first, InputIter last)
        {
            this->size_     = std::distance(first, last);
            this->capacity_ = this->size_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(first, last, begin());
        }

        /**
         * @brief Copy constructor. Create a %vector by copying the contents
         * from %other vector
         *
         * @param other Another %vector to copy
         */
        constexpr vector(const vector &other)
        {
            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(other.begin(), other.end(), begin());
        }

        /**
         * @brief Move constructor. Create a %vector by moving the ownership
         * from %other vector.
         *
         * @param other Another rvalue %vector to move
         */
        constexpr vector(vector &&other) noexcept
        {
            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            this->container_ = std::move(other.container_);
            other.capacity_  = 0;
            other.size_      = 0;
        }

        /**
         * @brief Create a %vector with a C-style array initialization.
         *
         * @param initial_list Contents to create.
         */
        constexpr vector(std::initializer_list<value_type> initial_list)
        {
            this->size_     = initial_list.size();
            this->capacity_ = this->size_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(initial_list.begin(), initial_list.end(), begin());
        }

        /**
         * @brief Assign overloaded operator. Assign %other vector without
         * compromising.
         *
         * @param other Another %vector to assign
         */
        constexpr vector &operator=(const vector &other)
        {
            if (std::addressof(other) == this)
                return *this;

            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            if (this->container_ == nullptr)
                this->container_
                    = std::make_unique<value_type[]>(this->capacity_);

            std::copy(other.begin(), other.end(), begin());

            return *this;
        }

        /**
         * @brief Move %other vector to an existing vector
         *
         * @param other Another %vector to move
         */
        constexpr vector &operator=(vector &&other) noexcept
        {
            if (std::addressof(other) == this)
                return *this;

            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            this->container_ = std::move(other.container_);
            other.size_      = 0;

            return *this;
        }

        /**
         * @brief Assign an existing %vector to a C-style array
         *
         * @param init_list Contents to assign
         */
        constexpr vector &operator=(std::initializer_list<value_type> init_list)
        {
            this->container_.reset();

            this->size_     = init_list.size();
            this->capacity_ = this->size_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(init_list.begin(), init_list.end(), begin());

            return *this;
        }

        // Iterators
        /**
         * @brief Read/write iterator pointing to the first element in the
         * %vector
         */
        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return this->container_.get();
        }

        /**
         * @brief Read-only iterator pointing to the first element in the
         * %vector
         */
        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return this->container_.get();
        }

        /**
         * @brief Read/write iterator pointing to the end in the %vector
         */
        [[nodiscard]] constexpr iterator end() noexcept
        {
            return this->container_.get() + this->size_;
        }

        /**
         * @brief Read-only iterator pointing to the end in the %vector
         */
        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return this->container_.get() + this->size_;
        }

        /**
         * @brief Read-only iterator pointing to the first element in the
         * %vector
         */
        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return this->container_.get();
        }

        /**
         * @brief Read-only iterator pointing to the end in the %vector
         */
        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return this->container_.get() + this->size_;
        }

        /**
         * @brief Read/write iterator pointing to the first element in the
         * %vector in reversed order.
         */
        [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }

        /**
         * @brief Read/write iterator pointing to the end in the %vector in
         * reversed order
         */
        [[nodiscard]] constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }

        /**
         * @brief  Read-only iterator pointing to the first element in the
         * %vector in reversed order
         */
        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        /**
         * @brief  Read-only iterator pointing to the end in the %vector in
         * reversed order
         */
        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        /**
         * @brief  Read-only iterator pointing to the first element in the
         * %vector in reversed order
         */
        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        /**
         * @brief  Read-only iterator pointing to the end in the %vector in
         * reversed order
         */
        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        // Member access
        /**
         * @brief Access to the content held at the position %i in the %vector
         * without range-checking
         *
         * @param i Index in the %vector
         */
        constexpr reference operator[](size_type i)
        {
            return *(this->container_.get() + i);
        }

        /**
         * @brief Access to the read-only content held at the position %i in the
         * %vector without range-checking
         *
         * @param i Index in the %vector
         */
        constexpr const_reference operator[](size_type i) const
        {
            return *(this->container_.get() + i);
        }

        /**
         * @brief Access to the valid content held at position %i in the
         * %vector, or throw otherwise
         *
         * @param i Index in the %vector
         */
        constexpr reference at(size_type i)
        {
            if (i >= this->size_)
                throw std::out_of_range("Index is out of range, which is "
                                        + std::to_string(this->size_));

            return this->operator[](i);
        }

        /**
         * @brief Access to the read-only valid content held at position %i in
         * the %vector, or throw otherwise
         *
         * @param i Index in the %vector
         */
        constexpr const_reference at(size_type i) const
        {
            if (i >= this->size_)
                throw std::out_of_range("Index is out of range, which is "
                                        + std::to_string(this->size_));

            return this->operator[](i);
        }

        /**
         * @brief Access to the first content in the %vector
         */
        constexpr reference front() { return this->operator[](0); }

        /**
         * @brief Read-only access to the first content in the %vector
         */
        constexpr const_reference front() const { return this->operator[](0); }

        /**
         * @brief Access to the last content in the %vector
         */
        constexpr reference back() { return this->operator[](this->size_ - 1); }

        /**
         * @brief Read-only access to the last content in the %vector
         */
        constexpr const_reference back() const
        {
            return this->operator[](this->size_ - 1);
        }

        /**
         * @brief Return the raw C-style pointer of the %vector
         */
        constexpr pointer data() noexcept { return this->container_.get(); }

        /**
         * @brief Return the raw read-only C-style pointer of the %vector
         */
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

        // Capacities
        /**
         * @brief Return the total existing element in the %vector
         */
        constexpr size_type size() const noexcept { return this->size_; }

        /**
         * @brief Return the total elements %vector can hold
         */
        constexpr size_type max_size() const noexcept
        {
            return this->capacity_;
        }

        /**
         * @brief Check if the %vector is empty or not, i.e no element.
         */
        constexpr bool empty() const noexcept { return size() == 0; }

        // Modifiers
        /**
         * @brief Remove all the content in the  %vector
         */
        constexpr void clear() noexcept
        {
            this->container_.reset();
            size_ = 0;
        }

        /**
         * @brief Insert a newly-constructed element before the pos
         *
         * @param pos Position iterator to insert
         * @param args Args List of arguments matching the constructors
         */
        template <typename... Args>
        constexpr iterator emplace(const_iterator pos, Args &&...args)
        {
            const std::size_t n = pos - cbegin();

            if (pos == cend())
                traits_t::construct(alloc_,
                                    this->container_.get() + this->size_,
                                    std::forward<Args>(args)...);
            else
            {

                for (auto curr = this->size_, prev = curr - 1; curr != n;
                     curr--, prev--)
                {
                    this->container_[curr] = this->container_[prev];
                }
                traits_t::construct(alloc_, this->container_.get() + n,
                                    std::forward<Args>(args)...);
            }

            if (++this->size_ >= capacity_)
            {
                expand_();
            }

            return this->container_.get() + n;
        }

        /**
         * @brief Insert a newly-constructed element at the end of %vector
         *
         * @param args List of arguments matching the constructors
         */
        template <typename... Args>
        constexpr iterator emplace_back(Args &&...args)
        {
            return this->emplace(end(), std::forward<Args>(args)...);
        }

        /**
         * @brief Insert an existing element at the end of %vector
         *
         * @param value Element to insert
         */
        constexpr void push_back(const_reference value)
        {
            this->emplace_back(value);
        }

        /**
         * @brief Insert an rvalue existing element at the end of %vector
         *
         * @param value Element to insert
         */
        constexpr void push_back(value_type &&value)
        {
            this->emplace_back(std::move(value));
        }

        /**
         * @brief Insert an existing element %value at %pos in %vector
         *
         * @param pos Position iterator to insert
         * @param value Value to insert
         */
        constexpr iterator insert(const_iterator pos, const_reference value)
        {
            return this->emplace(pos, value);
        }

        /**
         * @brief Move an element %value at %pos in %vector
         *
         * @param pos Position iterator to insert
         * @param value Value to insert
         */
        constexpr iterator insert(const_iterator pos, value_type &&value)
        {
            return this->emplace(pos, std::move(value));
        }

        /**
         * @brief Insert %total number of elements %value at %pos
         *
         * @param pos Position iterator to insert
         * @param total Total number of elements to insert
         * @param value Same value to insert
         */
        constexpr iterator insert(const_iterator pos, size_type total,
                                  const value_type &value)
        {
            this->size_ += total;
            auto n = pos - cbegin();

            for (auto curr = this->size_ - 1, prev = curr - total; prev >= n;
                 --curr, --prev)
            {
                this->container_[curr] = this->container_[prev];
            }

            std::fill_n(begin() + n, total, value);

            if (size() >= max_size())
                expand_();

            return begin() + n;
        }

        /**
         * @brief Insert a C-style %init_list at %pos
         *
         * @param pos Position iterator to insert
         * @param init_list C-style array of elements to insert
         */
        constexpr iterator insert(const_iterator                    pos,
                                  std::initializer_list<value_type> init_list)
        {
            this->size_ += init_list.size();
            auto n = pos - cbegin();

            for (auto curr = this->size_ - 1, prev = curr - init_list.size();
                 prev >= n; --curr, --prev)
            {
                this->container_[curr] = this->container_[prev];
            }

            std::copy(init_list.begin(), init_list.end(), begin() + n);

            if (size() >= max_size())
                expand_();

            return begin() + n;
        }

        /**
         * @brief Insert the contents from range [%first, %last) at %pos
         *
         * @param pos Position iterator to insert
         * @param first Inclusive lower-bound iterator of range to insert
         * @param last Exlcusive upper-bound iterator of range to insert
         */
        template <typename InputIter,
                  typename = std::_RequireInputIter<InputIter>>
        constexpr iterator insert(const_pointer pos, InputIter first,
                                  InputIter last)
        {
            difference_type dist = std::distance(first, last);
            this->size_ += dist;
            auto n = pos - cbegin();

            for (auto curr = this->size_ - 1, prev = curr - dist; prev >= n;
                 --curr, --prev)
            {
                this->container_[curr] = this->container_[prev];
            }

            std::copy(first, last, begin() + n);

            if (size() >= max_size())
                expand_();

            return begin() + n;
        }

        /**
         * @brief Erase the element at %pos from %vector
         *
         * @param pos Position iterator to erase
         */
        constexpr iterator erase(const_iterator pos)
        {
            auto n = pos - cbegin();

            for (auto curr = n, next = curr + 1; next < this->size_;
                 curr++, next++)
            {
                container_[curr] = container_[next];
            }

            this->size_--;

            return begin() + n;
        }

        /**
         * @brief Erase the elements from range [%first, %last) from %vector
         *
         * @param first Inclusive lower-bound iterator of range to erase
         * @param last Exclusive upper-bound iterator of  range to erase
         */
        constexpr iterator erase(const_iterator first, const_iterator last)
        {
            auto n = first - cbegin();
            auto d = std::distance(first, last);

            for (auto curr = n, next = curr + d; next < this->size_;
                 ++curr, ++next)
            {
                container_[curr] = container_[next];
            }

            this->size_ -= d;

            return begin() + n;
        }

        /**
         * @brief Erase the last element from %vector
         */
        constexpr void pop_back() { this->erase(end() - 1); }

        /**
         * @brief Resize %vector for efficient memory
         *
         * @param s Size to efficiently resize
         */
        constexpr void resize(size_type s)
        {
            while (s > max_size())
                expand_();

            this->size_        = s;
            auto new_container = std::make_unique<value_type[]>(s);
            std::copy(begin(), begin() + s, new_container.get());

            this->container_ = std::move(new_container);
        }

        /**
         * @brief Exchange the content and information between two %vector
         *
         * @param other Another vector swap
         */
        constexpr void swap(vector &other) noexcept
        {
            std::swap(this->size_, other.size_);
            std::swap(this->capacity_, other.capacity_);
            this->container_.swap(other.container_);
        }

    private:
        std::allocator<value_type> alloc_;
        using traits_t = std::allocator_traits<decltype(alloc_)>;
        std::unique_ptr<value_type[]> container_;
        size_type                     capacity_ = 8;
        size_type                     size_     = 0;

        constexpr void expand_() noexcept
        {
            size_type old_capacity = this->capacity_;
            this->capacity_        = old_capacity << 1;

            std::unique_ptr<value_type[]> new_container
                = std::make_unique<value_type[]>(this->capacity_);

            std::copy(begin(), end(), new_container.get());

            this->container_ = std::move(new_container);
        }
    };

    template <class T>
    constexpr bool operator==(const vector<T> &x, const vector<T> &y)
    {
        if (x.size() != y.size())
            return false;

        for (const auto x_start = x.begin(), y_start = y.begin();
             x_start != x.end() && y_start != y.end(); ++x_start, ++y_start)
        {
            if (*x_start != *y_start)
                return false;
        }

        return true;
    }

    template <class T>
    constexpr bool operator!=(const vector<T> &x, const vector<T> &y)
    {
        return !(x == y);
    }

    template <class T>
    constexpr std::ostream &operator<<(std::ostream &out, const vector<T> &x)
    {
        for (auto begin = std::cbegin(x); begin != std::cend(x); ++begin)
            std::cout << *begin << " ";
        std::cout << std::endl;

        return out;
    }

    template <std::size_t I, class T>
    constexpr T &get(vector<T> &a) noexcept
    {
        static_assert(I < a.size(), "index out of range");
        return a[I];
    }

    template <std::size_t I, class T>
    constexpr T &&get(vector<T> &&a) noexcept
    {
        static_assert(I < a.size(), "index out of range");
        return std::move(a[I]);
    }

    template <std::size_t I, class T>
    constexpr const T &get(const vector<T> &a) noexcept
    {
        static_assert(I < a.size(), "index out of range");
        return a[I];
    }

    template <std::size_t I, class T>
    constexpr const T &&get(const vector<T> &&a) noexcept
    {
        static_assert(I < a.size(), "index out of range");
        return std::move(a[I]);
    }
}; // namespace opendsa

#endif
