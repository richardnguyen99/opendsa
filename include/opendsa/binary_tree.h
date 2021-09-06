/**
 * @file binary_tree.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief A binary tree and its variants implementations
 * @version 0.1
 * @date 2021-09-04
 *
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>

namespace opendsa
{
    template <typename T>
    struct binary_node_
    {
    public:
        binary_node_() noexcept
            : parent_node_(nullptr), left_node_(nullptr), right_node_(nullptr)
        {
        }

        explicit binary_node_(const T &value) noexcept
            : parent_node_(nullptr), value_(value), left_node_(nullptr),
              right_node_(nullptr)
        {
        }

        explicit binary_node_(T &&value) noexcept
            : parent_node_(nullptr), value_(std::move(value)),
              left_node_(nullptr), right_node_(nullptr)
        {
        }

        explicit binary_node_(const T &value, binary_node_<T> *parent_ptr)
            : parent_node_(parent_ptr), left_node_(nullptr),
              right_node_(nullptr), value_(value)
        {
        }

        binary_node_(const binary_node_<T> &node) = delete;
        binary_node_ &operator=(const binary_node_<T> &node) = delete;

        ~binary_node_()
        {
            std::move(left_node_);
            std::move(right_node_);
        }

        binary_node_<T> *parent_node_;
        T                value_;

        std::unique_ptr<binary_node_<T>> left_node_;
        std::unique_ptr<binary_node_<T>> right_node_;
    };

    /**
     * @brief A base iterator class. This only extracts as well as gives acces
     * to the nodes in a binary tree. It does not handle traversal logic. To do
     * that, please use other derived classes, or implement your own
     */
    template <typename T, typename U = std::remove_cv_t<T>>
    struct iterator_base_
    {
        using value_type        = U;
        using pointer_type      = U *;
        using reference_type    = U &;
        using size_type         = std::size_t;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator_base_() = default;

        iterator_base_(binary_node_<T> *node) : node_(node)
        {
            if (node->parent_node_ != nullptr)
            {
                if (node->parent_node_->left_node_.get() == node)
                {
                    this->is_left_node_ = true;
                }
            }
        }

        reference_type operator*() const noexcept
        {
            return this->node_->value_;
        }

        pointer_type operator->() const noexcept
        {
            return &(this->node_->value_);
        }

        binary_node_<T> *node_         = nullptr;
        bool             is_left_node_ = false;
    };

    template <typename T>
    class pre_order_iterator : public iterator_base_<T>
    {
    public:
        pre_order_iterator() = default;

        pre_order_iterator(binary_node_<T> *node) : iterator_base_<T>(node) {}

        bool operator==(const pre_order_iterator<T> &iterator) const
        {
            return (*this->node_ == iterator.node_);
        }

        bool operator!=(const pre_order_iterator<T> &iterator) const
        {
            return (*this->node_ != iterator.node_);
        }

        pre_order_iterator<T> &operator++()
        {
            // Continue to go down on the left nodes
            if (this->node_->left_node_ != nullptr)
            {
                this->node_         = this->node_->left_node_.get();
                this->is_left_node_ = true;
            }

            else if (this->node_->right_node_ != nullptr)
            {
                this->node_         = this->node_->right_node_.get();
                this->is_left_node_ = false;
            }

            // If there is no left node, go to right node
            else if (this->node_->parent_node_->right_node_ != nullptr
                     && this->is_left_node_)
            {
                this->node_ = this->node_->parent_node_->right_node_.get();
                this->is_left_node_ = false;
            }

            // Otherwise, traverse back to their parent node and continue on the
            // parent node's sibling
            else if (this->node_->parent_node_->parent_node_ != nullptr)
            {
                bool parent_node_left = false;

                if (this->node_->parent_node_->parent_node_->left_node_.get()
                    == this->node_->parent_node_)
                {
                    parent_node_left = true;
                }

                if (!parent_node_left)
                {
                    this->node_ = nullptr;
                }

                else
                {
                    this->node_ = this->node_->parent_node_->parent_node_
                                      ->right_node_.get();
                    this->is_left_node_ = false;
                }
            }

            else
            {
                this->node_ = nullptr;
            }

            return *this;
        }

    private:
    };

    template <typename T>
    class binary_tree
    {
    public:
        using value_type      = T;
        using reference_type  = T &;
        using pointer_type    = T *;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using node = binary_node_<T>;

        /**
         * @brief Default constructor. Constructs an empty binary tree
         *
         */
        binary_tree() noexcept : root_(nullptr) {}

        /**
         * @brief Copy constructor. Constructs a binary tree with a copied value
         *
         * @param value  copied value to initialize
         */
        explicit binary_tree(const reference_type value) : root_(value) {}

        /**
         * @brief Move constructor. Constructs a binary tree with a rvalue value
         *
         * @param value  moved value to initialize
         */
        explicit binary_tree(T &&value)
            : root_(std::make_unique<node>(std::move(value)))
        {
        }

        /**
         * @brief Copy constructor. Constructs a binary tree with a copy value
         * of the other source
         *
         * @param other copied other binary tree to initialize
         */
        binary_tree(const binary_tree<T> &other)
        {
            if (other.root_ == nullptr)
                this->root_ == nullptr;
            else
                copy_binary_tree_(this->root_, other.root_);
        }

        /**
         * @brief Returns the iterator to the first element of the binary tree
         */
        pre_order_iterator<T> begin() noexcept
        {
            return pre_order_iterator<T>(this->root_.get());
        }

        /**
         * @brief  Returns the iterator to the end element of the binary tree
         */
        pre_order_iterator<T> end() noexcept { return pre_order_iterator<T>(); }

        /**
         * @brief Returns the raw pointer to the root of the binary tree
         */
        node root() noexcept { return this->root_.get(); }

        /**
         * @brief Inserts a new copied node to the left of the node pointed by
         * pos
         *
         * @param pos  iterator to the left of which the content is inserted
         * @param value copied value to insert
         */
        template <typename Iter>
        Iter insert_left(Iter pos, const reference_type value)
        {

            return Iter(insert_left_(pos, value));
        }

        /**
         * @brief Inserts a new rvalue node to the left of the node pointed by
         * pos
         *
         * @param pos  iterator to the left of which the content is inserted
         * @param value rvalue value to insert
         */
        template <typename Iter>
        Iter insert_left(Iter pos, T &&value)
        {
            return Iter(insert_left_(pos, value));
        }

        /**
         * @brief Inserts a new copied node to the right of the node pointed by
         * pos
         *
         * @param pos  iterator to the right of which the content is inserted
         * @param value copied value to insert
         */
        template <typename Iter>
        Iter insert_right(Iter pos, const reference_type value)
        {

            return Iter(insert_right_(pos, value));
        }

        /**
         * @brief Inserts a new rvalue node to the right of the node pointed by
         * pos
         *
         * @param pos  iterator to the right of which the content is inserted
         * @param value rvalue value to insert
         */
        template <typename Iter>
        Iter insert_right(Iter pos, T &&value)
        {
            return Iter(insert_right_(pos, value));
        }

    private:
        std::unique_ptr<node> root_;

        void copy_binary_tree_(std::unique_ptr<node> &root,
                               std::unique_ptr<node> &source)
        {
            if (source == nullptr)
            {
                root == nullptr;
            }

            root = std::make_unique<node>(source->value_, source->parent_node_);

            copy_binary_tree_(root->left_node_, source->left_node_);
            copy_binary_tree_(root->right_node_, source->right_node_);
        }

        template <typename Iter, typename... Args>
        node *insert_left_(Iter pos, Args &&...args)
        {
            if (pos.node_->left_node_ == nullptr)
            {
                auto new_node
                    = std::make_unique<node>(std::forward<Args>(args)...);
                new_node->parent_node_ = pos.node_;

                pos.node_->left_node_ = std::move(new_node);
            }

            return pos.node_->left_node_.get();
        }

        template <typename Iter, typename... Args>
        node *insert_right_(Iter pos, Args &&...args)
        {
            if (pos.node_->right_node_ == nullptr)
            {
                auto new_node
                    = std::make_unique<node>(std::forward<Args>(args)...);
                new_node->parent_node_ = pos.node_;

                pos.node_->right_node_ = std::move(new_node);
            }

            return pos.node_->right_node_.get();
        }
    };
} // namespace opendsa
