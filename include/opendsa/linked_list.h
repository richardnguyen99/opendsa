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
#include <memory>
#include <optional>

namespace opendsa
{
    /**
     * @brief Basic helper node class to work with singly-linked list
     */
    template <typename T>
    struct singly_node_base
    {
        singly_node_base() = default;

        singly_node_base(const T &value)
            : next_ptr_(), data_{std::make_optional<T>(value)}
        {
        }

        singly_node_base(singly_node_base &&node)
            : next_ptr_{std::move(node.next_ptr_)}, data_{std::make_optional<T>(
                                                        node.data_.value())}
        {
        }

        singly_node_base(const singly_node_base &) = delete;
        singly_node_base &operator=(const singly_node_base &) = delete;

        singly_node_base &operator=(singly_node_base &&node) noexcept
        {
            next_ptr_ = std::move(node.next_ptr_);
            data_     = std::make_optional<T>(node.data_.value());
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
        std::optional<T>                     data_     = std::nullopt;
    };

    template <typename T>
    struct singly_node_iterator
    {
        using value_type        = T;
        using pointer           = T *;
        using reference         = T &;
        using difference_type   = std::ptrdiff_t;
        using size_type         = std::size_t;
        using iterator_category = std::forward_iterator_tag;
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
        using alloc_   = std::allocator<T>;
        using traits_t = std::allocator_traits<alloc_>;

        using value_type      = T;
        using pointer         = typename traits_t::const_pointer;
        using reference       = T &;
        using const_pointer   = typename traits_t::pointer;
        using const_reference = const T &;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using iterator       = singly_node_iterator<T>;
        using const_iterator = singly_node_iterator<const T>;

        /**
         * @brief Default cosntructor. Constructs an empty container
         */
        singly_linked_list() = default;

        /**
         * @brief Constructs the container with count copies of elements with
         * value
         *
         * @param count Number of
         * @param value
         */
        singly_linked_list(size_type count, const T &value) {}

        template <typename InputIterator,
                  typename = std::_RequireInputIter<InputIterator>>
        singly_linked_list(InputIterator first, InputIterator last)
        {
            range_initialize_(first, last);
        }

    private:
        std::unique_ptr<singly_node_base<T>> head_ = nullptr;
        singly_node_base<T> *                tail_ = nullptr;

        void push_back_(const T &d)
        {
            if (head_ == nullptr)
            {
                head_ = std::make_unique<singly_node_base<T>>(d);
                tail_ = head_.get();
            }
            else
            {
                tail_->next_ptr_ = std::make_unique<singly_node_base<T>>(d);
                tail_            = tail_->next_ptr_.get();
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
    };
} // namespace opendsa
