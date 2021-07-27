/**
 * @file linked_list.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief  Header for linked list implementations
 * @version 0.1
 * @date 2021-07-17
 *
 * @copyright Copyright (c) 2021
 *
 * This header contains several classess, interfaces and functions to create,
 * custom and work with data structures, linked list.
 */

#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <utility>

namespace opendsa
{
    /**
     * @brief Basic helper node class to work with singly-linked list
     */
    template <typename T>
    struct singly_node_base
    {
        singly_node_base() = default;

        singly_node_base(const T &value) : next_ptr_(), data_{value} {}

        singly_node_base(singly_node_base &&node)
            : next_ptr_{std::move(node.next_ptr_)}, data_{node.data_}
        {
        }

        singly_node_base(const singly_node_base &node) = delete;
        singly_node_base &operator=(const singly_node_base &) = delete;

        singly_node_base &operator=(singly_node_base &&node) noexcept
        {
            next_ptr_ = std::move(node.next_ptr_);
            data_     = node.data_;
            node      = nullptr;
            return *this;
        }

        ~singly_node_base()
        {
            for (std::unique_ptr<singly_node_base<T>> current
                 = std::move(next_ptr_);
                 current != nullptr; current = std::move(current->next_ptr_))
                ;
        }

        std::unique_ptr<singly_node_base<T>> next_ptr_ = nullptr;
        T                                    data_;
    };

    template <typename T, typename U = std::remove_cv_t<T>>
    struct singly_node_iterator
    {
        using value_type        = T;
        using pointer           = T *;
        using reference         = T &;
        using difference_type   = std::ptrdiff_t;
        using size_type         = std::size_t;
        using iterator_category = std::forward_iterator_tag;
        using self              = singly_node_iterator<T>;

        singly_node_base<U> *node_ptr_;

        singly_node_iterator() : node_ptr_(nullptr) {}

        singly_node_iterator(singly_node_base<U> *node_ptr)
            : node_ptr_(node_ptr)
        {
        }

        singly_node_iterator(const singly_node_iterator<U> &iterator) noexcept
            : node_ptr_(iterator.node_ptr_)
        {
        }

        reference operator*() const noexcept { return node_ptr_->data_; }

        pointer operator->() const noexcept { return node_ptr_; }

        self &operator++() noexcept
        {
            node_ptr_ = node_ptr_->next_ptr_.get();
            return *this;
        }

        self operator++(int) noexcept
        {
            singly_node_iterator tmp(*this);
            node_ptr_ = node_ptr_->next_ptr_.get();
            return tmp;
        }

        friend bool operator==(const self &x, const self &y) noexcept
        {
            return x.node_ptr_ == y.node_ptr_;
        }

        friend bool operator!=(const self &x, const self &y) noexcept
        {
            return x.node_ptr_ != y.node_ptr_;
        }
    };

    /**
     * @brief A singly-linked list container with linear time element access and
     * constant time insertion/deletion.
     *
     * A singly-linked list is a container that allows insertion and deletion to
     * have a constant time performance regardless position. However, its
     * trade-off is the lookup time is linear, which means it doesn't allow
     * indexing like operator[].
     */
    template <typename T>
    class singly_linked_list
    {
    public:
        using Alloc_   = std::allocator<T>;
        using traits_t = std::allocator_traits<Alloc_>;

        using value_type      = T;
        using pointer         = typename traits_t::const_pointer;
        using reference       = T &;
        using const_pointer   = typename traits_t::pointer;
        using const_reference = const T &;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using iterator       = singly_node_iterator<T>;
        using const_iterator = singly_node_iterator<const T>;

        // === Begin MEMBERS

        /**
         * @brief Default cosntructor. Constructs an empty container
         */
        singly_linked_list() = default;

        /**
         * @brief Constructs the container with count copies of elements with
         * value
         *
         * @param count Number of elements
         * @param value Value to insert
         */
        singly_linked_list(size_type count, const T &value)
        {
            fill_initialize_(count, value);
        }

        /**
         * @brief Copy constructor. Constructs the container with the copy of
         * the contents of other.
         *
         * @param other Another singly-linked list
         */
        singly_linked_list(const singly_linked_list &other)
        {
            range_initialize_(other.begin(), other.end());
        }

        /**
         * @brief Move constructor. Constructs the container with the contents
         * of other and move the other
         *
         * @param other RRvalue singly-linked list
         */
        singly_linked_list(singly_linked_list &&other) = default;

        /**
         * @brief Construct a new singly linked list object
         *
         * @param initial_list Initializer list
         */
        singly_linked_list(std::initializer_list<T> initial_list)
        {
            range_initialize_(initial_list.begin(), initial_list.end());
        }

        /**
         * @brief Constructs the container with the contents of the range.
         *
         * @param first The beginning input iterator
         * @param last  The end input iterator
         */
        template <typename InputIterator,
                  typename = std::_RequireInputIter<InputIterator>>
        singly_linked_list(InputIterator first, InputIterator last)
        {
            range_initialize_(first, last);
        }

        /**
         * @brief
         *
         * @param list
         * @return singly_linked_list&
         */
        singly_linked_list &operator=(const singly_linked_list &list)
        {
            if (std::addressof(list) != this)
            {
            }
        }

        singly_linked_list &operator=(singly_linked_list &&list) noexcept {}

        singly_linked_list &operator=(std::initializer_list<T> list) {}

        ~singly_linked_list()
        {
            // for (std::unique_ptr<singly_node_base<T>> current
            // = std::move(head_);
            // current != nullptr; current = std::move(current->next_ptr_))
            // ;
        }
        // === End MEMBERS

        // === Begin ACCESS

        /**
         * @brief Returns a reference to the first element in the container
         *
         * The expression `front() is equivalent to `*begin()`.
         */
        reference front() { return *head_.get(); }

        /**
         * @brief Returns a read-only reference to the first element in the
         * container
         *
         * The expression `front() is equivalent to `*begin()`.
         */
        const_reference front() const { return *head_.get(); }

        // === End ACCESS

        // === Begin ITERATORS

        /**
         * @brief Iterator to the first element
         */
        iterator begin() noexcept { return iterator(head_.get()); }

        /**
         * @brief Read-only iterator to the first element
         */
        const_iterator begin() const noexcept
        {
            return const_iterator(head_.get());
        }

        /**
         * @brief Read-only iterator to the first element
         */
        const_iterator cbegin() const noexcept
        {
            return const_iterator(head_.get());
        }

        /**
         * @brief Iterator to the end of the container (Not the last element)
         */
        iterator end() noexcept { return iterator(nullptr); }

        /**
         * @brief Read-only iterator to the end of the container (Not the last
         * element)
         */
        const_iterator end() const noexcept { return const_iterator(nullptr); }

        /**
         * @brief Read-only iterator to the end of the container (Not the last
         * element)
         */
        const_iterator cend() const noexcept { return const_iterator(nullptr); }

        // === End ITERATORS

        // == Begin MODIFIERS

        /**
         * @brief Inserts a  new in-place constructed element after the
         * specified postion in the container.
         *
         * @param pos Iterator after which the new element will be constructed.
         * @param args List of arguments to forward to the constructor of the
         * element.
         */
        template <typename... Args>
        iterator emplace_after(const_iterator pos, Args &&...args)
        {
            return iterator(insert_after_(pos, std::forward<Args>(args)...));
        }

        /**
         * @brief Insert value after the iterator
         *
         * @param pos iterator after which the content will be inserted
         * @param value value to insert
         */
        iterator insert_after(const_iterator pos, const T &value)
        {
            return iterator(insert_after_(pos, value));
        }

        /**
         * @brief Insert value after the iterator
         *
         * @param pos iterator after which the content will be inserted
         * @param value value to insert
         */
        iterator insert_after(const_iterator pos, T &&value)
        {
            return iterator(insert_after_(pos, std::move(value)));
        }

        /**
         * @brief Insert count copies of the value after the pos iterator
         *
         * @param pos iterator after which the contents will be inserted
         * @param count number of copies to insert
         * @param value values to insert
         */
        iterator insert_after(const_iterator pos, size_type count,
                              const T &value)
        {
            return iterator(insert_range_after_(pos, count, value));
        }

        /**
         * @brief Insert elements from the range after the pos iterator. Cannot
         * use this on its own.
         *
         * @param pos iterator after which the contents will be inserted
         * @param first Iterator to the beginning of the range
         * @param last  Iteraotr to the end of the range
         */
        template <typename InputIterator,
                  typename = std::_RequireInputIter<InputIterator>>
        iterator insert_after(const_iterator pos, InputIterator first,
                              InputIterator last)
        {
            return iterator(insert_range_after_(pos, first, last));
        }

        /**
         * @brief Insert elements from the contents of initializer list.
         *
         * @param pos iterator after which the contents will be inserted
         * @param list Initializer list to insert values from.
         * @return iterator
         */
        iterator insert_after(const_iterator pos, std::initializer_list<T> list)
        {
            return iterator(insert_range_after_(pos, list.begin(), list.end()));
        }

        void push_back(const T &value) { push_back_(value); }

        void push_front(const T &value) { push_front_(value); }

        /**
         * @brief Inserts a new in-place constructed element to the beginning of
         * the container.
         *
         * @param args List of arguments to construct the new element.
         */
        template <typename... Args>
        reference emplace_front(Args &&...args)
        {
            push_front_(std::forward<Args>(args)...);
            return front();
        }

        /**
         * @brief Removes the first elemt of the container.
         */
        void pop_front()
        {
            if (head_ != nullptr)
            {
                head_ = std::move(head_->next_ptr_);
            }
        }

        /**
         * @brief Removes the element in pos
         *
         * @param pos iterator to the element precdeding the element to remove
         */
        iterator erase_after(const_iterator pos)
        {
            return iterator(erase_after_(pos.node_ptr_));
        }

        /**
         * @brief Removes the elements from range first unitl last
         *
         * @param first  iterator to the first of the range
         * @param last  iterator to the last of the range
         */
        iterator erase_after(const_iterator first, const_iterator last)
        {
            return iterator(
                erase_range_after_(first.node_ptr_, last.node_ptr_));
        }

        // === End MODIFIERS

        // === Begin OPERATIONS

        void swap(singly_linked_list<T> &ll) noexcept
        {
            singly_node_base<T> *tmp_tail
                = static_cast<singly_node_base<T> *>(tail_);
            tail_    = ll.tail_;
            ll.tail_ = tmp_tail;

            head_.swap(ll.head_);
        }

        void sort() { sort_(head_.get(), nullptr); }

        /**
         * @brief
         *
         * @tparam Compare
         * @param comp
         */
        template <typename Compare>
        void sort(Compare comp)
        {
            if (head_ == nullptr)
                return;
        }

        void merge(const singly_linked_list<T> &ll)
        {
            singly_linked_list<T> result;
            merge_(head_.get(), tail_->next_ptr_.get(), ll.head_.get(),
                   ll.tail_->next_ptr_.get(), result);

            head_->data_     = result.head_->data_;
            head_->next_ptr_ = std::move(result.head_->next_ptr_);
            tail_            = result.tail_;
        }

        void reverse() noexcept
        {
            singly_linked_list<T> tmp;
            for (singly_node_base<T> *node = head_.get(); node != nullptr;
                 node                      = node->next_ptr_.get())
            {
                tmp.push_front(node->data_);
            }

            head_ = std::move(tmp.head_);
            tail_ = tmp.tail_;
        }

        // === End OPERATIONS

    private:
        Alloc_ alloc;

        std::unique_ptr<singly_node_base<T>> head_ = nullptr;
        singly_node_base<T> *                tail_ = nullptr;

        template <typename... Args>
        void push_back_(Args &&...args)
        {
            if (head_ == nullptr)
            {
                head_ = std::make_unique<singly_node_base<T>>(
                    std::forward<Args>(args)...);
                tail_ = head_.get();
            }
            else
            {
                tail_->next_ptr_ = std::make_unique<singly_node_base<T>>(
                    std::forward<Args>(args)...);
                tail_ = tail_->next_ptr_.get();
            }
        }

        template <typename... Args>
        void push_front_(Args &&...args)
        {
            if (head_ == nullptr)
            {
                head_ = std::make_unique<singly_node_base<T>>(
                    std::forward<Args>(args)...);
                tail_ = head_.get();
            }
            else
            {
                auto new_head = std::make_unique<singly_node_base<T>>(
                    std::forward<Args>(args)...);
                new_head->next_ptr_ = std::move(head_);
                head_               = std::move(new_head);
            }
        }

        template <typename InputIterator>
        void range_initialize_(InputIterator first, InputIterator last)
        {

            for (; first != last; ++first)
            {
                push_back_(*first);
            }
        }

        void fill_initialize_(size_type count, const T &value)
        {
            for (size_type i = 0; i < count; ++i)
            {
                push_back_(value);
            }
        }

        template <typename... Args>
        singly_node_base<T> *insert_after_(const_iterator pos, Args &&...args)
        {
            auto new_node = std::make_unique<singly_node_base<T>>(
                std::forward<Args>(args)...);

            new_node->next_ptr_      = std::move(pos.node_ptr_->next_ptr_);
            pos.node_ptr_->next_ptr_ = std::move(new_node);

            if (pos++ == cend())
            {
                tail_ = pos.node_ptr_->next_ptr_.get();
            }

            return pos.node_ptr_;
        }

        singly_node_base<T> *
        insert_range_after_(const_iterator pos, size_type count, const T &value)
        {
            for (size_type i = 0; i < count; ++i, ++pos)
            {
                insert_after_(pos, value);
            }

            return pos.node_ptr_;
        }

        template <typename InputIterator,
                  typename = std::_RequireInputIter<InputIterator>>
        singly_node_base<T> *insert_range_after_(const_iterator pos,
                                                 InputIterator  first,
                                                 InputIterator  last)
        {
            for (; first != last; ++first, ++pos)
            {
                insert_after_(pos, *first);
            }
        }

        singly_node_base<T> *erase_after_(singly_node_base<T> *pos)
        {
            singly_node_base<T> *current
                = static_cast<singly_node_base<T> *>(pos->next_ptr_.get());

            pos->next_ptr_ = std::move(current->next_ptr_);

            return pos->next_ptr_.get();
        }

        singly_node_base<T> *erase_range_after_(singly_node_base<T> *first,
                                                singly_node_base<T> *last)
        {
            singly_node_base<T> *current
                = static_cast<singly_node_base<T> *>(first->next_ptr_.get());

            for (; current->next_ptr_.get() != last;)
            {
                current = current->next_ptr_.get();
            }

            first->next_ptr_ = std::move(current->next_ptr_);

            return last;
        }

        singly_node_base<T> *copy_(singly_node_base<T> *first,
                                   singly_node_base<T> *last,
                                   singly_node_base<T> *dest)
        {
            for (; first != last;
                 first = first->next_ptr_.get(), dest = dest->next_ptr_.get())
            {
                dest = first;
            }
        }

        void merge_(singly_node_base<T> *first1, singly_node_base<T> *first2,
                    singly_node_base<T> *second1, singly_node_base<T> *second2,
                    singly_linked_list<T> &result)
        {
            // Returns if we hits the base cases
            if (first1 == nullptr)
                return;
            if (second1 == nullptr)
                return;

            // result to return, current to move nodes without losing result

            // Initlize the head of result
            if (first1->data_ < second1->data_)
            {
                result.push_back(first1->data_);
                first1 = first1->next_ptr_.get();
            }
            else
            {
                result.push_back(second1->data_);
                second1 = second1->next_ptr_.get();
            }

            // Iterate one linked-list at a time.
            // If one runs out before the other, push back the other's values
            for (; first1 != first2;)
            {
                if (second1 == second2)
                {
                    for (; first1 != first2; first1 = first1->next_ptr_.get())
                    {
                        result.push_back(first1->data_);
                    }

                    break;
                }

                if (first1->data_ < second1->data_)
                {
                    result.push_back(first1->data_);
                    first1 = first1->next_ptr_.get();
                }
                else
                {
                    result.push_back(second1->data_);
                    second1 = second1->next_ptr_.get();
                }
            }

            for (; second1 != second2; second1 = second1->next_ptr_.get())
            {
                result.push_back(second1->data_);
            }
        }

        void sort_(singly_node_iterator<T> begin, singly_node_iterator<T> end)
        {
            // Returns immediately if the size of the container is 0 or 1.
            if (std::distance(begin, end) <= 1)
                return;

            // Finds the middle start in the container using the technique
            // of merge sort. FINDING THE MIDDLE KEY

            // Hare poiniter will traverse twice fast as the tortoise
            // pointer.
            singly_node_iterator hare     = begin;
            singly_node_iterator tortoise = begin;

            // If the hare pointer hit the end, or nullptr, the tortoise
            // pointer will be pointing the middle start.
            while (hare != end)
            {
                hare++;

                if (hare != end)
                {
                    tortoise++;
                    hare++;
                }
            }

            // The range is [first, last)
            sort_(begin, tortoise);
            sort_(tortoise, end);

            singly_linked_list<T> result;

            // Merge the two sub linked lists and store it
            merge_(begin.node_ptr_, tortoise.node_ptr_, tortoise.node_ptr_,
                   end.node_ptr_, result);

            // Copy back to the original list
            std::copy(std::begin(result), std::end(result), begin);
        }
    };
} // namespace opendsa
