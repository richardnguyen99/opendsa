/**
 * @file vector.h
 * @author Richar Nguyen (richard.ng0616@gmail.com)
 * @brief Header for dynamic array
 * @version 0.1
 * @date 2021-12-12
 *
 * @copyright Copyright (c) 2021
 */

#ifndef _DYNAMIC_ARRAY_H
#define _DYNAMIC_ARRAY_H

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace opendsa
{
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
        constexpr vector() noexcept : size_(0)
        {
            this->container_ = std::make_unique<value_type[]>(this->capacity_);
        }

        constexpr vector(size_type total, const_reference value)
            : size_(total), capacity_(total)
        {
            this->container_ = std::make_unique<value_type[]>(total);

            std::fill(begin(), end(), value);
        }

        constexpr explicit vector(size_type total) : size_(0), capacity_(total)
        {
            this->container_ = std::make_unique<value_type[]>(total);
        }

        template <class InputIter, typename = std::_RequireInputIter<InputIter>>
        constexpr vector(InputIter first, InputIter last)
        {
            this->size_     = std::distance(first, last);
            this->capacity_ = this->size_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(first, last, begin());
        }

        constexpr vector(const vector &other)
        {
            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(other.begin(), other.end(), begin());
        }

        constexpr vector(vector &&other) noexcept
        {
            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            this->container_ = std::move(other.container_);
            other.capacity_  = 0;
            other.size_      = 0;
        }

        constexpr vector(std::initializer_list<value_type> initial_list)
        {
            this->size_     = initial_list.size();
            this->capacity_ = this->size_;

            this->container_ = std::make_unique<value_type[]>(this->capacity_);

            std::copy(initial_list.begin(), initial_list.end(), begin());
        }

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

        constexpr vector &operator=(vector &&other)
        {
            if (std::addressof(other) == this)
                return *this;

            this->size_     = other.size_;
            this->capacity_ = other.capacity_;

            this->container_ = std::move(other.container_);
            other.size_      = 0;

            return *this;
        }

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
        constexpr iterator begin() noexcept { return this->container_.get(); }

        constexpr const_iterator begin() const noexcept
        {
            return this->container_.get();
        }

        constexpr iterator end() noexcept
        {
            return this->container_.get() + this->size_;
        }

        constexpr const_iterator end() const noexcept
        {
            return this->container_.get() + this->size_;
        }

        constexpr const_iterator cbegin() const noexcept
        {
            return this->container_.get();
        }

        constexpr const_iterator cend() const noexcept
        {
            return this->container_.get() + this->size_;
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
            if (i >= this->size_)
                throw std::out_of_range("Index is out of range, which is "
                                        + std::to_string(this->size_));

            return this->operator[](i);
        }

        constexpr const_reference at(size_type i) const
        {
            if (i >= this->size_)
                throw std::out_of_range("Index is out of range, which is "
                                        + std::to_string(this->size_));

            return this->operator[](i);
        }

        constexpr reference front() { return this->operator[](0); }

        constexpr const_reference front() const { return this->operator[](0); }

        constexpr reference back() { return this->operator[](this->size_ - 1); }

        constexpr const_reference back() const
        {
            return this->operator[](this->size_ - 1);
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

        // Capacities
        constexpr size_type size() const noexcept { return this->size_; }

        constexpr size_type max_size() const noexcept
        {
            return this->capacity_;
        }

        constexpr bool empty() const noexcept { return size() == 0; }

        // Modifiers
        constexpr void clear() noexcept
        {
            this->container_.reset();
            size_ = 0;
        }

        template <typename... Args>
        constexpr iterator emplace(const_iterator pos, Args &&...args)
        {
            const auto n = pos - cbegin();

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

        template <typename... Args>
        constexpr iterator emplace_back(Args &&...args)
        {
            this->emplace(end(), std::forward<Args>(args)...);
        }

        constexpr void push_back(const_reference value)
        {
            this->emplace_back(value);
        }

        constexpr void push_back(value_type &&value)
        {
            this->emplace_back(std::move(value));
        }

        constexpr iterator insert(const_iterator pos, const_reference value)
        {
            return this->emplace(pos, value);
        }

        constexpr iterator insert(const_iterator pos, value_type &&value)
        {
            return this->emplace(pos, std::move(value));
        }

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

        constexpr void pop_back() { this->erase(end() - 1); }

        constexpr void resize(size_type s)
        {
            while (s > max_size())
                expand_();

            this->size_        = s;
            auto new_container = std::make_unique<value_type[]>(s);
            std::copy(begin(), begin() + s, new_container.get());

            this->container_ = std::move(new_container);
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
}; // namespace opendsa

#endif
