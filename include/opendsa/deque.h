/**
 * @file include/opendsa/deque.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Header for deque (deck) implemenetation
 * @version 0.1
 * @date 2021-07-03
 *
 * @copyright Copyright (c) 2021
 *
 * This header contains several classes, interfaces
 * and functions to create, custom and work with the
 * data structure deque (deck). They're supposed to
 * replicate the standard `deque` from the C++ STL.
 * It's supposed to make the C++ STL deque easier
 * to understand, not to replace it IRL.
 *
 * @see
 * https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_deque.h
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <optional>

namespace opendsa
{
    /**
     * @brief Returns the number element of each node
     *
     * @param size Number of elements
     *
     * In the STL C++, this function is used to check whether the size of each
     * element is big or not, and therefore allocate the proper size for each
     * node. if elements are std::size_t, which has 8 bytes, the buffer_size()
     * will return 64.
     *
     * To keep it simple, we don't need to worry about memory management, so we
     * want this function return 8 for the size of each node.
     */
    constexpr inline std::size_t buffer_size() noexcept { return 8; }
    /**
     * @brief An iterator class to support the opendsa::deque implementation
     *
     * This class supports traversing, adding, removing, assigning and many
     * other functionalities and operations to work with the opendsa::deque.
     */
    template <class T, class U = std::remove_cv_t<T>>
    struct deque_iterator
    {
        using value_type        = U;
        using reference         = U &;
        using pointer           = U *;
        using size_type         = std::size_t;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        using const_iterator    = deque_iterator<const U>;
        // Use std::remove_cv_t to remove const constraints so that
        // we can work with both iterator and const_iterator at the same time.
        using iterator    = deque_iterator<std::remove_cv_t<T>>;
        using map_pointer = std::unique_ptr<std::remove_cv_t<T>[]> *;

        pointer     current_;
        pointer     first_;
        pointer     last_;
        map_pointer node_;

        deque_iterator(pointer current, map_pointer new_node) noexcept
            : current_{current}, first_{(*new_node).get()},
              last_{(*new_node).get() + buffer_size()}, node_{new_node}
        {
        }

        deque_iterator() noexcept : current_(), first_(), last_(), node_() {}

        deque_iterator(const iterator &iter) noexcept
            : current_(iter.current_), first_(iter.first_), last_(iter.last_),
              node_(iter.node_)
        {
        }

        deque_iterator &operator=(const deque_iterator &) = default;

        deque_iterator &operator+=(difference_type n) noexcept
        {
            const difference_type offset = n + (current_ - first_);
            if (offset >= 0 && offset < difference_type(buffer_size()))
            {
                current_ += n;
            }
            else
            {
                const difference_type node_offset
                    = offset > 0
                          ? offset / difference_type(buffer_size())
                          : -difference_type((-offset - 1) / buffer_size()) - 1;

                set_node_(node_ + node_offset);
                current_
                    = first_
                      + (offset - node_offset * difference_type(buffer_size()));
            }

            return *this;
        }

        deque_iterator &operator-=(difference_type n) noexcept
        {
            return *this += -n;
        }

        reference operator*() const noexcept { return *current_; }

        reference operator[](difference_type n) const noexcept
        {
            return *(*this + n);
        }

        pointer operator->() const noexcept { return current_; }

        deque_iterator &operator++() noexcept
        {
            ++current_;
            if (current_ == last_)
            {
                set_node_(node_ + 1);
                current_ = first_;
            }

            return *this;
        }

        deque_iterator operator++(int) noexcept
        {
            deque_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        deque_iterator &operator--() noexcept
        {
            if (current_ = first_)
            {
                set_node_(node_ - 1);
                current_ = last_;
            }
            --current_;
            return *this;
        }

        deque_iterator operator--(int) noexcept
        {
            deque_iterator tmp = *this;
            --*this;
            return tmp;
        }

        void set_node_(map_pointer node) noexcept
        {
            node_  = node;
            first_ = (*node_).get();
            last_  = first_ + buffer_size();
        }

        deque_iterator<U> const_cast_() const noexcept
        {
            return deque_iterator<U>(current_, node_);
        }

        friend deque_iterator operator+(const deque_iterator &x,
                                        difference_type       n) noexcept
        {
            deque_iterator tmp = x;
            tmp += n;
            return tmp;
        }

        friend deque_iterator operator+(difference_type       n,
                                        const deque_iterator &x) noexcept
        {
            return x + n;
        }

        friend deque_iterator operator-(const deque_iterator &x,
                                        difference_type       n) noexcept
        {
            deque_iterator tmp = x;
            tmp -= n;
            return tmp;
        }

        friend deque_iterator operator-(difference_type       n,
                                        const deque_iterator &x) noexcept
        {
            return x - n;
        }

        friend difference_type operator-(const deque_iterator &x,
                                         const deque_iterator &y) noexcept
        {
            return difference_type(buffer_size())
                       * (x.node_ - y.node_ - int(x.node_ != 0))
                   + (x.current_ - x.first_) + (y.last_ - y.current_);
        }

        friend bool operator==(const deque_iterator &a,
                               const deque_iterator &b) noexcept
        {
            return a.current_ == b.current_;
        }

        friend bool operator!=(const deque_iterator &a,
                               const deque_iterator &b) noexcept
        {
            return !(b == a);
        }
    };

    /**
     * @brief An experimental deque data structure inspired by the std::deque
     *
     * @tparam T  Type of elements
     *
     * opendsa::deque is supposed to replicate the std::deque for learning
     * purpose. That means there are many simplification and shortcuts to make
     * the deque data structure more understandable while still maintains its
     * functionalities to work with the C++ STL and opendsa headers.
     *
     * To use or know the standard version from C++ STL, see at:
     * https://en.cppreference.com/w/cpp/container/deque
     *
     * If you really need the actual implementation of deque, go check at:
     * https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_deque.h
     */
    template <typename T>
    class deque
    {
    public:
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using value_type             = T;
        using reference              = T &;
        using const_reference        = const T &;
        using pointer                = T *;
        using const_pointer          = const T *;
        using iterator               = deque_iterator<T>;
        using const_iterator         = deque_iterator<const T>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // Member functions ===
        /**
         * @brief Default constructor. Use the intialized values of class
         * members.
         */
        deque() { initialize_map_(0); }

        /**
         * @brief Constructs the container with count copies of elements with
         * value
         *
         * @param count Number of elements
         * @param value Copied value of elements
         */
        explicit deque(size_type count, const value_type &value)
        {
            initialize_map_(count);
            fill_initialize_(value);
        }

        /**
         * @brief Constructs the container with the contents of the range.
         *
         * @tparam InputIterator Should be matched with deque_iterator or be
         * legal iterators
         * @param first Input iterator for start
         * @param last  Input iterator for finish
         */
        template <typename InputIterator,
                  typename = std::_RequireInputIter<InputIterator>>
        explicit deque(InputIterator first, InputIterator last)
        {
            initialize_map_(0);
            range_initialize_(first, last, std::input_iterator_tag());
        }

        /**
         * @brief Copy constructor. Constructs the container with the copy of
         * the contents of ther
         *
         * @param other Other opendsa::deque
         */
        deque(const deque &other)
        {
            std::uninitialized_copy(other.begin(), other.end(), start_ptr_);
        }

        /**
         * @brief Move constructor. Constructs the container with the contents
         * of other using move semantics.
         *
         * @param other Other rvalue-reference
         */
        deque(deque &&other) : deque(std::move(other)) {}

        /**
         * @brief Constructs the container with contents of the initializer list
         *
         * @param init Initializer list
         */
        deque(std::initializer_list<T> init)
        {
            initialize_map_(0);
            range_initialize_(init.begin(), init.end(),
                              std::random_access_iterator_tag());
        }

        /**
         * @brief Destroy the deque object
         */
        ~deque() { destroy_data_(begin(), end()); }

        /**
         * @brief Copy assignment operator. Replaces the contents with a copy of
         * the contents of other.
         *
         * @param other another container to use as data source
         */
        deque &operator=(const deque &other)
        {
            if (std::addressof(other) != this)
            {
                if (size() >= other.size())
                {
                    // Copy the content from range to the new start pointer
                    auto cursor
                        = std::copy(other.begin(), other.end(), start_ptr_);

                    // Erase elements in the old pointers
                    erase_at_end_(cursor);
                }
                else
                {
                    const_iterator mid
                        = other.begin() + difference_type(size());
                    std::copy(other.begin(), mid, start_ptr_);
                    range_insert_(finish_ptr_, mid, other.end());
                }
            }

            return *this;
        }

        /**
         * @brief Move assignment operator. Replaces the contents with the
         * contents of other using moving semantics
         *
         * @param other another container to se as data source
         */
        deque &operator=(deque &&other) noexcept {}

        /**
         * @brief Replaces the contents with the contents of the intializer list
         * init
         *
         * @param init Intializer list
         */
        deque &operator=(std::initializer_list<T> init) {}

        /**
         * @brief Replaces the contents with count copies of value
         *
         * @param count new size of the container
         * @param value intial value of elements
         */
        void assign(size_type count, const_reference value) {}

        /**
         * @brief Replaces the contents with copies in the range.
         *
         * @tparam InputIterator Legal Input Iterator
         * @param first An input Iterator for the start of range
         * @param last An input Iterator for the end of range
         */
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
        }

        /**
         * @brief Replaces the contents with the elements from the initializer
         * init
         *
         * @param init Intializer list
         */
        void assign(std::initializer_list<T> init) {}

        // Iterators ===
        /**
         * @brief Iterator to the beginning data in the deque. Use with
         * std::begin()
         */
        iterator begin() noexcept { return start_ptr_; }

        /**
         * @brief Iterator to the beginning data in the deque. Use with
         * std::cbegin()
         */
        const_iterator begin() const noexcept { return start_ptr_; }

        /**
         * @brief Read-only iterator to the beginning data in the deque. Use by
         * its own.
         */
        const_iterator cbegin() const noexcept { return start_ptr_; }

        /**
         * @brief Iterator to the ending data in the deque. Use with std::end()
         */
        iterator end() noexcept { return finish_ptr_; }

        /**
         * @brief Iterator to the ending data in the deque. Use with std::cend()
         */
        const_iterator end() const noexcept { return finish_ptr_; }

        /**
         * @brief Read-only iterator to the ending data in the deque. Use by its
         * own
         */
        const_iterator cend() const noexcept { return finish_ptr_; }

        /**
         * @brief Iterator to the ending data as the first iterator in the
         * deque. Use with std::rbegin()
         */
        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(finish_ptr_);
        }

        /**
         * @brief Iterator to the ending data as the first iterator in the
         * deque. Use with std::crbegin()
         */
        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(finish_ptr_);
        }

        /**
         * @brief Read-only iterator to the ending data as the first iterator in
         * the deque. Use by its own
         */
        const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(finish_ptr_);
        }

        /**
         * @brief Iterator to the beginning data as the ending iterator in the
         * deque. Use with std::rend()
         */
        reverse_iterator rend() noexcept
        {
            return reverse_iterator(start_ptr_);
        }

        /**
         * @brief Iterator to the beginning data as the ending iterator in the
         * deque. Use with std::crend()
         */
        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(start_ptr_);
        }

        /**
         * @brief Read-only iterator to the beginning data as the ending
         * iterator in the deque. Use by its own.
         */
        const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(start_ptr_);
        }

        // Element access ===
        /**
         * @brief Access to the data in the deque
         *
         * @param count  The index of the element.
         *
         * This function have the same functionality as operator[], but it has
         * range checked and throws out exception if the check fails.
         */
        reference at(size_type count) {}

        /**
         * @brief Read-only access to the data in the deque
         *
         * @param count  The index of the element.
         *
         * This function have the same functionality as operator[], but it has
         * range checked and throws out exception if the check fails.
         */
        const_reference at(size_type count) const {}

        /**
         * @brief Access to the data in the deque
         *
         * @param pos  The index of the element
         *
         * This function allows array-style data access with no range checked.
         * For safer lookup function, check at()
         */
        reference operator[](size_type pos) noexcept
        {
            return start_ptr_[difference_type(pos)];
        }

        /**
         * @brief Read-only access to the data in the deque
         *
         * @param pos  The index of the element
         *
         * This function allows array-style data access with no range checked.
         * For safer lookup function, check at()
         */
        const_reference operator[](size_type pos) const noexcept
        {
            return start_ptr_[difference_type(pos)];
        }

        /**
         * @brief Access to the first data in the deque
         */
        reference front() noexcept { return *begin(); }

        /**
         * @brief Read-only access to the first data in the deque
         */
        const_reference front() const noexcept { return *begin(); }

        /**
         * @brief Access to the end data in the deque
         */
        reference back() noexcept { return *end(); }

        /**
         * @brief Read-only access to the end data in the deque
         */
        const_reference back() const noexcept { return *end(); }
        // Capacity ===
        /**
         * @brief Checks if the deque is empty or not
         *
         * It will check if the begin() iterator is equal to the end() iterator.
         */
        bool empty() const noexcept { return finish_ptr_ == start_ptr_; }

        /**
         * @brief Returns the number of elements in the container
         *
         * It will compute the distance between the begin() iterator and the
         * end() iterator using std::distance
         */
        size_type size() const noexcept { return finish_ptr_ - start_ptr_; }

        // Modifiers ===
        /**
         * @brief Prune all elements from the container.
         *
         * This function will remove elements from the container, returns size()
         * to zero, invalidates any references, pointers and iterators referring
         * to contained elements.
         */
        void clear() noexcept { erase_at_end_(begin()); }

        /**
         * @brief Push an in-place constructed object before pos
         *
         * @param pos A const_iterator into the deque
         * @param args Argument list.
         */
        template <class... Args>
        iterator emplace(const_iterator pos, Args &&...args)
        {
            if (pos.current_ == start_ptr_.current_)
            {
                emplace_front(std::forward<Args>(args)...);
                return start_ptr_;
            }
            else if (pos.current_ == finish_ptr_.current_)
            {
                emplace_back(std::forward<Args>(args)...);
                iterator tmp = finish_ptr_;
                --tmp;
                return tmp;
            }
            else
                return insert_(pos.const_cast_(), std::forward<Args>(args)...);
        }

        /**
         * @brief Push an in-place constructed object back to the deque
         *
         * @param args Argument list for constructor
         */
        template <class... Args>
        reference emplace_back(Args &&...args)
        {
            // Check if there is still space for new element
            if (finish_ptr_.current_ != finish_ptr_.last_ - 1)
            {
                // std::allocator<T>::construct is marked as deprecated in c++17
                // and removed in c++20, so we use allocator_traits
                std::allocator<T> alloc;
                using traits_t = std::allocator_traits<decltype(alloc)>;

                traits_t::construct(alloc, finish_ptr_.current_,
                                    std::forward<Args>(args)...);

                ++finish_ptr_.current_;
            }
            else
            {
                push_back_(std::forward<Args>(args)...);
            }

            return back();
        }

        /**
         * @brief Push an in-place constructed to the front of the deque
         *
         * @param args Argument list for constructor
         */
        template <class... Args>
        reference emplace_front(Args &&...args)
        {
            // Check if there is still space for new element
            if (start_ptr_.current_ != start_ptr_.first_)
            {
                // std::allocator<T>::construct is marked as deprecated in c++17
                // and removed in c++20, so we use allocator_traits
                std::allocator<T> alloc;
                using traits_t = std::allocator_traits<decltype(alloc)>;

                traits_t::construct(alloc, start_ptr_.current_ - 1,
                                    std::forward<Args>(args)...);

                --start_ptr_.current_;
            }
            else
            {
                push_front_(std::forward<Args>(args)...);
            }

            return front();
        }

        /**
         * @brief Inserts the given value into deque before specified iterator
         *
         * @param pos  A const_iterator into the deque
         * @param value Value to be inserted.
         */
        iterator insert(const_iterator pos, const_reference value)
        {
            if (pos.current_ == start_ptr_.current_)
            {
                push_front(value);
                return start_ptr_;
            }
            else if (pos.current_ = finish_ptr_.current_)
            {
                push_back(value);
                iterator tmp = finish_ptr_;
                --tmp;
                return tmp;
            }
            else
                return insert_(pos.const_cast_(), value);
        }

        /**
         * @brief Inserts the given value into deque before specified iterator
         *
         * @param pos  A const_iterator into the deque
         * @param value RValue to be inserted.
         */
        iterator insert(const_iterator pos, value_type &&value)
        {
            return emplace(pos, std::move(value));
        }

        /**
         * @brief Inserts count copies of value into the deque before the pos
         * iterator
         *
         * @param pos A const_iterator into the deque
         * @param count Number of elements to be inserted
         * @param value Value of elements to be inserted
         */
        iterator insert(const_iterator pos, size_type count,
                        const_reference value)
        {
        }

        /**
         * @brief Inserts the contents of the eange into the deque before the
         * pos
         *
         * @tparam InputIterator Legal Input Iterator
         * @param pos A const_iterator into the deque
         * @param first An input iterator as the beginning of the range
         * @param last An input iterator as the ending of the range
         */
        template <class InputIterator>
        iterator insert(const_pointer pos, InputIterator first,
                        InputIterator last)
        {
        }

        /**
         * @brief Inserts an intializer list into the deque before the the pos
         *
         * @param pos A const_iterator into the deque
         * @param init An initializer list to be inserted
         */
        iterator insert(const_iterator                    pos,
                        std::initializer_list<value_type> init)
        {
        }

        /**
         * @brief Removes element at the given position
         *
         * @param pos  Iterator pointing to the element to be erased
         */
        iterator erase(const_iterator pos) {}

        /**
         * @brief Remove a range of elements
         *
         * @param first Iterator pointing the first element to be erased
         * @param last Iterator pointing the last element to be erased
         */
        iterator erase(const_iterator first, const_iterator last) {}

        /**
         * @brief Add data to the end of the deque
         *
         * @param value Data to be added
         */
        void push_back(const_reference value)
        {
            // Check if there is still space for new element
            if (finish_ptr_.current_ != finish_ptr_.last_ - 1)
            {
                // std::allocator<T>::construct is marked as deprecated in c++17
                // and removed in c++20, so we use allocator_traits
                std::allocator<T> alloc;
                using traits_t = std::allocator_traits<decltype(alloc)>;

                traits_t::construct(alloc, finish_ptr_.current_, value);

                ++finish_ptr_.current_;
            }
            else
            {
                push_back_(value);
            }
        }

        /**
         * @brief Add data to the end of the deque
         *
         * @param value Data to be added
         */
        void push_back(T &&value) { emplace_back(std::move(value)); }

        /**
         * @brief Removes the last element
         */
        void pop_back() noexcept {}

        /**
         * @brief Add data to the begin of the deque
         *
         * @param value Data to be added
         */
        void push_front(const_reference value)
        {
            // Check if there is still space for new element
            if (start_ptr_.current_ != start_ptr_.first_)
            {
                // std::allocator<T>::construct is marked as deprecated in c++17
                // and removed in c++20, so we use allocator_traits
                std::allocator<T> alloc;
                using traits_t = std::allocator_traits<decltype(alloc)>;

                traits_t::construct(alloc, start_ptr_.current_ - 1, value);

                --start_ptr_.current_;
            }
            else
            {
                push_front_(value);
            }
        }

        /**
         * @brief Add data to the begin of the deque
         *
         * @param value Data to be added
         */
        void push_front(T &&value) { emplace_front(std::move(value)); }

        /**
         * @brief Removes the first element
         */
        void pop_front() noexcept {}

        /**
         * @brief Swaps data with another deque
         *
         * @param other Another deque with the same element type
         */
        void swap(deque &other) noexcept {}

    protected:
        void range_check_(size_type count) const {}

    private:
        // A deque is typically implemented as a vector of fix-sized arrays.
        // In this case, node_pointers are fix-sized array, and map_pointer
        // is the vector of node_pointers.
        using node_pointer = std::unique_ptr<T[]>;
        using map_pointer  = std::unique_ptr<node_pointer[]>;

        // A standard intial size for a deque
        static constexpr size_type initial_map_size = 8;

        map_pointer map_;
        size_type   map_size_;

        deque_iterator<T> start_ptr_;
        deque_iterator<T> finish_ptr_;

        /**
         * @brief Initialize the map pointer for containing data in deque
         *
         * @param count Number of elements
         *
         * This private method creates an initial map, like a dynamic array of
         * fix-sized arrays, according to the given number. This will only
         * allocate space and set pointers for tracking the occupied nodes. To
         * fill the map with contents, check fill_initialize_() or
         * range_initialize_().
         */
        void initialize_map_(size_type count)
        {
            // Calculate how many node needed for this container
            // E.g: initial size  = 8 -> map_size = 2.
            const size_type num_nodes = (count / buffer_size()) + 1;

            map_size_ = std::max(initial_map_size, num_nodes + 2);
            map_      = std::make_unique<node_pointer[]>(map_size_);

            auto dstart = (map_.get() + (map_size_ - num_nodes) / 2);
            auto dend   = dstart + num_nodes;

            for (auto current = dstart; current < dend; ++current)
            {
                *current = std::make_unique<T[]>(8);
            }

            start_ptr_.set_node_(dstart);
            finish_ptr_.set_node_(dend - 1);
            start_ptr_.current_  = start_ptr_.first_;
            finish_ptr_.current_ = finish_ptr_.first_ + count % buffer_size();
        }

        /**
         * @brief Fill the container with the value. Use after the container is
         * initialized
         *
         * @param value Value of elements to be inserted
         */
        void fill_initialize_(const_reference value)
        {
            for (auto curr = start_ptr_.node_; curr < finish_ptr_.node_; ++curr)
            {
                std::uninitialized_fill_n((*curr).get(), buffer_size(), value);
            }

            std::uninitialized_fill_n(finish_ptr_.first_,
                                      finish_ptr_.current_ - finish_ptr_.first_,
                                      value);
        }

        /**
         * @brief Fill the container with the contents of range.
         *
         * @param first Iterator for the beginning of the range
         * @param last  Iterator for the end of the range
         */
        template <typename InputIter>
        void range_initialize_(InputIter first, InputIter last,
                               std::input_iterator_tag)
        {
            for (; first != last; ++first)
            {
                emplace_back(*first);
            }
        }

        template <typename... Args>
        void push_back_(Args &&...args)
        {
            *(finish_ptr_.node_ + 1) = std::make_unique<T[]>(buffer_size());
            // std::allocator<T>::construct is marked as deprecated in c++17
            // and removed in c++20, so we use allocator_traits
            std::allocator<T> alloc;
            using traits_t = std::allocator_traits<decltype(alloc)>;

            traits_t::construct(alloc, finish_ptr_.current_,
                                std::forward<Args>(args)...);
            finish_ptr_.set_node_(finish_ptr_.node_ + 1);
            finish_ptr_.current_ = finish_ptr_.first_;
        }

        template <typename... Args>
        void push_front_(Args &&...args)
        {
            *(start_ptr_.node_ - 1) = std::make_unique<T[]>(buffer_size());
            start_ptr_.set_node_(start_ptr_.node_ - 1);
            start_ptr_.current_ = start_ptr_.last_ - 1;

            // std::allocator<T>::construct is marked as deprecated in c++17
            // and removed in c++20, so we use allocator_traits
            std::allocator<T> alloc;
            using traits_t = std::allocator_traits<decltype(alloc)>;

            traits_t::construct(alloc, start_ptr_.current_,
                                std::forward<Args>(args)...);
        }

        template <typename... Args>
        iterator insert_(iterator pos, Args &&...args)
        {
            value_type      insert_value(std::forward<Args>(args)...);
            difference_type index = pos - start_ptr_;

            // Shift the values to give space to insert the new value
            if (static_cast<size_type>(index) < size() / 2)
            {
                push_front(std::move(front()));
                iterator front1 = start_ptr_;
                ++front1;
                iterator front2 = front1;
                ++front2;
                pos           = start_ptr_ + index;
                iterator pos1 = pos;
                ++pos1;

                std::move(front2, pos1, front1);
            }
            else
            {
                push_back(std::move(back()));
                iterator back1 = finish_ptr_;
                --back1;
                iterator back2 = back1;
                --back2;
                pos = start_ptr_ + index;
                std::move(pos, back2, back1);
            }

            *pos = std::move(insert_value);
            return pos;
        }

        template <typename ForwardIterator>
        void insert_range_(iterator pos, ForwardIterator first,
                           ForwardIterator last, size_type n)
        {
            const difference_type front_elements = pos - start_ptr_;

            if (static_cast<size_type>(front_elements) < size() / 2)
            {
                new_elements_at_front_(n);
                auto     new_start = start_ptr_ - n;
                iterator old_start = start_ptr_;
                pos                = start_ptr_ + front_elements;

                if (front_elements >= difference_type(n))
                {
                    iterator n_start_ptr = (start_ptr_ + difference_type(n));
                    std::uninitialized_move(start_ptr_, n_start_ptr, new_start);
                    start_ptr_ = new_start;
                    std::move(n_start_ptr, pos, old_start);
                    std::copy(first, last, pos - difference_type(n));
                }
                else
                {
                    ForwardIterator mid = first;
                    std::advance(mid, difference_type(n) - front_elements);
                    auto tmp
                        = std::uninitialized_move(start_ptr_, pos, new_start);
                    std::uninitialized_copy(first, mid, tmp);
                    start_ptr_ = new_start;
                    std::copy(mid, last, old_start);
                }
            }
            else
            {
                new_elements_at_back_(n);
                auto new_finish = finish_ptr_ + n;

                iterator old_finish = finish_ptr_;

                const difference_type back_elements = size() - front_elements;
                pos = finish_ptr_ - back_elements;

                if (back_elements > difference_type(n))
                {
                    iterator n_finish_ptr = (finish_ptr_ - difference_type(n));
                    std::uninitialized_move(n_finish_ptr, finish_ptr_,
                                            finish_ptr_);
                    finish_ptr_ = new_finish;
                    std::move(pos, n_finish_ptr, old_finish);
                    std::copy(first, last, pos);
                }
                else
                {
                    ForwardIterator mid = first;
                    std::advance(mid, back_elements);
                    auto tmp = std::uninitialized_copy(mid, last, finish_ptr_);
                    std::uninitialized_move(pos, finish_ptr_, tmp);
                    finish_ptr_ = new_finish;
                    std::copy(first, mid, pos);
                }
            }
        }

        void destroy_data_(iterator first, iterator last)
        {
            // Destroy nodes with full elements
            for (auto node = first.node_ + 1; node < last.node_; ++node)
            {
                std::destroy((*node).get(), (*node).get() + buffer_size());
            }

            // Destroy ends
            if (first.node_ != last.node_)
            {
                std::destroy(first.current_, first.last_);
                std::destroy(last.first_, last.current_);
            }
            else
                std::destroy(first.current_, last.current_);
        }

        void destroy_nodes_(std::unique_ptr<std::remove_cv_t<T>[]> *start,
                            std::unique_ptr<std::remove_cv_t<T>[]> *finish)
        {
            for (auto n = start; n < finish; ++n)
                *n = nullptr;
        }

        void erase_at_end_(iterator pos)
        {
            destroy_data_(pos, end());
            destroy_nodes_(pos.node_ + 1, finish_ptr_.node_ + 1);
            finish_ptr_ = pos;
        }

        template <typename Iter>
        void range_insert_(iterator pos, Iter first, Iter last)
        {
            const size_type dist = std::distance(first, last);

            if (pos.current_ == start_ptr_.current_)
            {
                new_elements_at_front_(dist);
                auto new_start_ptr = start_ptr_ - dist;
                std::uninitialized_copy(first, last, new_start_ptr);
                start_ptr_ = new_start_ptr;
            }
            else if (pos.current_ == finish_ptr_.current_)
            {
                new_elements_at_back_(dist);
                auto new_finish_ptr = finish_ptr_ + dist;
                std::uninitialized_copy(first, last, finish_ptr_);
                finish_ptr_ = new_finish_ptr;
            }
            else
                insert_range_(pos, first, last, dist);
        }

        void new_elements_at_front_(size_type new_elements)
        {
            const size_type new_nodes
                = ((new_elements + buffer_size() - 1) / buffer_size());

            for (size_type i = 1; i <= new_nodes; ++i)
            {
                *(start_ptr_.node_ - i) = std::make_unique<T[]>(buffer_size());
            }
        }

        void new_elements_at_back_(size_type new_elements)
        {
            const size_type new_nodes
                = ((new_elements + buffer_size() - 1) / buffer_size());

            for (size_type i = 1; i <= new_nodes; i++)
            {
                *(finish_ptr_.node_ + i) = std::make_unique<T[]>(buffer_size());
            }
        }
    };

    // Non-member functions ===
    template <typename T>
    inline bool operator==(const deque<T> &a, const deque<T> &b)
    {
        return a.size() == b.size()
               && std::equal(a.begin(), a.end(), b.begin());
    }

    template <typename T>
    inline bool operator!=(const deque<T> &a, const deque<T> &b)
    {
        return !(a == b);
    }
} // namespace opendsa
