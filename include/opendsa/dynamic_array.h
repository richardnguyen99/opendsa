/**
 * @file include/opendsa/dynamic_array.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Headers for dynamical list-based container
 * @version 0.1
 * @date 2021-06-29
 *
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <iterator>
#include <memory>

namespace opendsa
{
    /**
     * @brief An experimental container for storing a flexible-size sequence of
     * elements.
     *
     * @tparam T Type of elements
     */
    template <typename T>
    class dynamic_array
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

            constexpr _internal_iterator &operator+(std::size_t number)
            {
                ptr_ += number;
                return *this;
            }

            constexpr _internal_iterator &operator+(std::size_t number) const
            {
                ptr_ += number;
                return *this;
            }

            constexpr _internal_iterator &operator-(std::size_t number)
            {
                ptr_ += number;
                return *this;
            }

            constexpr _internal_iterator &operator-(std::size_t number) const
            {
                ptr_ += number;
                return *this;
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

        /**
         * @brief Create a dynamic array with no elements
         *
         * This will create an empty dynamic array with initial capacity
         */
        constexpr dynamic_array() noexcept
            : capacity_{8}, size_{0}, container_{std::make_unique<value_type[]>(
                                          capacity_)}
        {
        }

        /**
         * @brief Create a dynamic array with elements of the same
         *
         * @param count The number of elements to initially create dynamic
         * arrays
         * @param value The value that each element holds
         */
        constexpr dynamic_array(size_type count, const_reference value)
            : capacity_{count}
        {
            container_ = std::make_unique<value_type[]>(capacity_);
            size_      = count;
            std::fill_n(std::begin(container_.get()), capacity_ - 1, value);
        }

        /**
         * @brief Create an empty dynamic array with intial capacity
         *
         * @param count The number of space to initially create dynamic arrays
         */
        constexpr explicit dynamic_array(size_type count) : capacity_{count}
        {
            size_      = count;
            container_ = std::make_unique<value_type[]>(capacity_);
        }

        /**
         * @brief Create a dynamic array from iterator range
         *
         * @tparam Iter
         * @param first Input iterator
         * @param second Input iterator
         */
        template <typename Iter>
        constexpr dynamic_array(Iter first, Iter last)
        {
            capacity_  = std::distance(first, last);
            size_      = capacity_;
            container_ = std::make_unique<T[]>(capacity_);

            for (Iter cursor = first; cursor != last; cursor++)
            {
                size_type idx = std::distance(first, cursor);

                container_[idx] = static_cast<value_type>(*cursor);
            }
        }

        /**
         * @brief Copy elements from the input array to the new one
         *
         * @param other Const input dynamic array
         */
        constexpr dynamic_array(const dynamic_array &other)
        {
            capacity_  = std::distance(other.cbegin(), other.cend());
            size_      = capacity_;
            container_ = std::make_unique<T[]>(capacity_);

            for (auto cursor = other.cbegin(); cursor != other.cend(); cursor++)
            {
                size_type idx = std::distance(other.cbegin(), cursor);

                container_[idx] = *cursor;
            }
        }

        /**
         * @brief Move elements from the inpout array to the new one
         *
         * @param other Input dynamic array
         */
        constexpr dynamic_array(dynamic_array &&other) noexcept
        {
            capacity_  = std::distance(std::begin(other), std::end(other));
            size_      = capacity_;
            container_ = std::move(other.container_);
        }

        /**
         * @brief Create a dynamic array with the contents of the initializer
         * list
         *
         * @param init Initializer list
         */
        constexpr dynamic_array(std::initializer_list<value_type> init)
        {
            capacity_  = init.size();
            size_      = 0;
            container_ = std::make_unique<T[]>(capacity_);

            for (auto cursor = init.begin(); cursor != init.end(); ++cursor)
            {
                size_type idx   = std::distance(init.begin(), cursor);
                container_[idx] = *cursor;
                size_++;
            }
        }

        ~dynamic_array()
        {
            size_      = 0;
            capacity_  = 0;
            container_ = nullptr;
        }

        // Capacity functions

        /**
         * @brief Returns the number of elements in the dynamic array
         */
        constexpr size_type size() const noexcept { return size_; }

        /**
         * @brief  Returns the number of space that the dynamic array has
         * allocated
         */
        constexpr size_type capacity() const noexcept { return capacity_; }

        /**
         * @brief Returns whether or not the dynamic array has no elements
         */
        constexpr bool empty() const noexcept { return size_ == 0; }

        /**
         * @brief Shrink the capacity to fit the size of the container and
         * destroy all element beyond the size
         */
        constexpr void shrink_to_fit()
        {
            if (size() == capacity())
                return;

            std::unique_ptr<value_type[]> tmp{container_.begin(),
                                              container_.end()};
            container_ = std::move(tmp);
        }

        // Iterators
        /**
         * @brief Returns the first iterator in the dynamic array
         */
        constexpr iterator begin() noexcept
        {
            return iterator{container_.get()};
        }

        /**
         * @brief Returns the first const iterator in the dynamic array
         */
        constexpr const_iterator cbegin() const noexcept
        {
            return const_iterator{container_.get()};
        }

        /**
         * @brief Returns the last iterator in the dynamic array
         */
        constexpr iterator end() noexcept
        {
            return iterator{container_.get() + size_};
        }

        /**
         * @brief Returns the last const iterator in the dynamic array
         */
        constexpr const_iterator cend() const noexcept
        {
            return const_iterator{container_.get() + size_};
        }

        /**
         * @brief Returns the last iterator in the dynamic array as the first
         * iterator
         */
        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator{iterator{container_.get() + size_}};
        }

        /**
         * @brief Returns the last const iterator in the dynamic array as the
         * first iterator
         */
        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator{
                const_iterator{container_.get() + size_}};
        }

        /**
         * @brief Returns the first iterator in the dynamic array as the last
         * iterator
         */
        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator{iterator{container_.get()}};
        }

        /**
         * @brief Returns the first const iterator in the dynamic array as the
         * last iterator
         */
        constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator{const_iterator{container_.get()}};
        }

        // Element access functions
        constexpr reference operator[](size_type index)
        {
            return container_[index];
        }

        constexpr const_reference operator[](size_type index) const
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

        // Modifier functions

        /**
         * @brief Erases every elements from the dynamic array, and size returns
         * zero while the capacity remains unchanged
         *
         */
        constexpr void clear() noexcept { size_ = 0; }

        /**
         * @brief Inserts a new value before the iterator
         *
         * @param pos Iterator that the new value is inserted before
         * @param value The value of the new iterator
         */
        constexpr iterator insert(const_iterator pos, const_reference value)
        {
            auto pdist = std::distance(cbegin(), pos);

            if (size() == capacity())
                expand();

            for (size_type cursor = size_, prev = cursor - 1; cursor > pdist;
                 --prev, --cursor)
            {
                container_[cursor] = container_[prev];
            }

            container_[pdist] = value;
            size_++;
        }

        /**
         * @brief Inserts a new value beofre the iterator with rvalue-reference
         *
         * @param pos Iterator that the new value is inserted before
         * @param value  The rvalue-reference that is about to be destroyed
         * @return constexpr iterator
         */
        constexpr iterator insert(const_iterator pos, T &&value)
        {
            auto pdist = std::distance(cbegin(), pos);

            if (size() == capacity())
                expand();

            for (size_type cursor = size_, prev = cursor - 1; cursor > pdist;
                 --prev, --cursor)
            {
                container_[cursor] = container_[prev];
            }

            container_[pdist] = std::move(value);
            size_++;
        }

        /**
         * @brief Inserts copies of the value before pos
         *
         * @param pos Iterator that the new values are inserted before
         * @param count The number of copied elements that is about to be
         * inserted
         * @param value The value of the new iterators
         */
        constexpr iterator insert(const_iterator pos, size_type count,
                                  const T &value)
        {
            auto pdist = std::distance(cbegin(), pos);

            // Move the container
            for (size_type cursor = size_ + count, prev = size_ - 1;
                 cursor > pdist; --prev, --cursor)
            {
                if (size() == capacity())
                    expand();

                container_[cursor] = container_[prev];
            }

            for (size_type cursor = pdist; cursor != cursor + count; ++cursor)
            {
                container_[cursor] = value;
                size_++;
            }
        }

        /**
         * @brief Inserts elements from the range before pos
         *
         * @param pos Iterator that the new values are inserted before
         * @param first The lower bound of range
         * @param last The upper bound of range
         */
        template <typename Iter>
        constexpr iterator insert(const_iterator pos, Iter first, Iter last)
        {
            auto distance = std::distance(first, last);
            auto pdist    = std::distance(cbegin(), pos);

            for (size_type cursor = size_ + distance, prev = size_ - 1;
                 cursor > pdist; --prev, --cursor)
            {
                if (size() == capacity())
                    expand();

                container_[cursor] = container_[prev];
                size_++;
            }

            for (size_type cursor = pdist; cursor < cursor + distance; ++cursor)
            {
                container_[cursor] = *(++first);
            }
        }

        /**
         * @brief Inserts elements from intial list before pos
         *
         * @param pos  Iterator that the new values are inserted before
         * @param initial_list Initial list to insert the values from
         */
        constexpr iterator insert(const_iterator           pos,
                                  std::initializer_list<T> initial_list)
        {
            auto pdist = std::distance(cbegin(), pos);
            auto count = initial_list.size();
            auto first = initial_list.begin();

            for (size_type cursor = size_ + count, prev = size_ - 1;
                 cursor > pos; --prev, --cursor)
            {
                if (size() == capacity())
                    expand();

                container_[cursor] = container_[prev];
            }

            for (size_type cursor = pdist; cursor != cursor + count; ++cursor)
            {
                container_[cursor] = *(++first);
                size_++;
            }
        }

        /**
         * @brief Erases the element at pos
         *
         * @param pos Iterator to the element to remove
         */
        constexpr iterator erase(const_iterator pos)
        {
            for (size_type next = pos + 1; next < size_; ++next, ++pos)
            {
                container_[pos] = container_[next];
            }

            size_--;
        }

        /**
         * @brief Erases the element in range
         *
         * @param first Iterator to the lower bound of range
         * @param last Iterator to the upper bound of range
         */
        constexpr iterator erase(const_iterator first, const_iterator last)
        {
            auto distance = std::distance(first, last);

            for (size_type next_of_range =
                     std::distance(std::begin(*this), last) + 1;
                 next_of_range < size_; ++next_of_range, ++first)
            {
                container_[std::distance(std::begin(*this), first)] =
                    container_[next_of_range];
            }

            size_ -= distance;
        }

        /**
         * @brief Pushes a copy of value to back of the dynamic array
         *
         * @param value The value of the element to insert
         */
        constexpr void push_back(const_reference value)
        {
            if (size_ = capacity_)
                expand();

            container_[size_] = value;
            size_++;
        }

        /**
         * @brief Pushes a rvalue-reference element to back of the dynamic array
         *
         * @param value The value of the element to be destroyed after inserting
         */
        constexpr void push_back(T &&value)
        {
            if (size_ = capacity_)
                expand();

            container_[size_] = std::move(value);
            size_++;
        }

        /**
         * @brief Removes the last element of the dynamic array
         */
        constexpr void pop_back() { size_--; }

        /**
         * @brief Resizes the size of the dynamic array to count
         *
         * @param count The new size of the container
         */
        constexpr void resize(size_type count) { size_ = count; }

        /**
         * @brief Resizes the size o the dynamic array to count with specified
         * values when resizing up
         *
         * @param count The new size of the container
         * @param value The additional copies of values
         */
        constexpr void resize(size_type count, const value_type &value)
        {
            auto old_size = size_;
            size_         = count;

            if (old_size < size_)
            {
                for (auto cursor = old_size; cursor < size_; ++cursor)
                    container_[cursor] = value;
            }
        }

        /**
         * @brief Exchange the container of two dynamic arrays
         *
         * @param other
         */
        constexpr void swap(dynamic_array &other) noexcept
        {
            container_.swap(other.container_);
        }

        // Non-member functions
        friend constexpr bool operator==(const dynamic_array<T> &lhs,
                                         const dynamic_array<T> &rhs) noexcept
        {
            return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
        }

        friend constexpr bool operator!=(const dynamic_array<T> &lhs,
                                         const dynamic_array<T> &rhs) noexcept
        {
            return !(lhs == rhs);
        }

    private:
        std::size_t          capacity_;
        std::size_t          size_;
        std::unique_ptr<T[]> container_;

        void expand()
        {
            auto new_capacity  = capacity_ << 1;
            auto new_container = std::make_unique<T[]>(new_capacity);

            std::copy_n(container_.get(), size_, new_container.get());

            container_ = std::move(new_container);
            capacity_  = new_capacity;
        }
    };

} // namespace opendsa

namespace std
{
    template <std::size_t index, class T>
    constexpr T &get(opendsa::dynamic_array<T> &arr) noexcept
    {
        return arr[index];
    }
} // namespace std
