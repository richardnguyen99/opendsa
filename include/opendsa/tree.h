/**
 * @file tree.h
 * @author Richard Nguyen
 * @brief
 * @version 0.1
 * @date 2021-06-24
 *
 * This header contains several implementations for
 * tree-based data structures, and methods that only work
 * on trees.
 *
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

namespace opendsa
{
    /**
     * @brief Internal tree node that holds value and pointers to other node
     *
     * This implementation only contains information about the node itself such
     * as holding value, pointers to other nodes, height, size, and so on. It
     * doesn't implement modifiers, access methods or iterators.
     */
    template <typename T>
    struct tree_node_
    {
        constexpr tree_node_() = default;

        constexpr tree_node_(const T &value)
            : value_(value), parent_node_(nullptr), child_nodes_()
        {
        }

        constexpr tree_node_(T &&value)
            : value_(std::move(value)), parent_node_(nullptr), child_nodes_()
        {
        }

        // Force to not use copy constructor in this implementation
        // since it is the job of tree.
        tree_node_(const tree_node_ &node) = delete;
        tree_node_ &operator=(const tree_node_ &node) = delete;

        ~tree_node_() { child_nodes_.clear(); }

        std::size_t size() const noexcept
        {
            // Each traversable node is counted as one.
            std::size_t node_number = 1;

            for (const std::unique_ptr<tree_node_<T>> &child :
                 this->child_nodes_)
            {
                node_number += child->size();
            }

            return node_number;
        }

        std::size_t height() const noexcept
        {
            // Use 0 here when the traversal node is already a leaf.
            std::size_t heights = 0;

            if (!this->child_nodes_.empty())
            {
                std::vector<std::size_t> sub_heights;

                std::transform(std::cbegin(this->child_nodes_),
                               std::cend(this->child_nodes_),
                               std::back_inserter(sub_heights),
                               [](const std::unique_ptr<tree_node_<T>> &child)
                               { return child->height(); });

                heights = *std::max_element(std::cbegin(sub_heights), std
                                            : cend(sub_heights))
                          + 1;
            }

            return heights;
        }

        std::size_t depth() const noexcept
        {
            // Use 0 here when hte traversal node is already a root
            std::size_t depth = 0;

            if (this->parent_node_ != nullptr)
            {
                depth = this->parent_node_->depth() + 1;
            }

            return depth;
        }

        std::size_t breadth() const noexcept
        {
            // Use 1 in case the node is already a leaf.
            if (this->child_nodes_.empty())
                return 1;

            std::size_t leaves = 0;
            for (const std::unique_ptr<tree_node_<T>> &child :
                 this->child_nodes_)
            {
                leaves += child->breadth();
            }

            return leaves;
        }

        T                                           value_;
        tree_node_<T> *                             parent_node_ = nullptr;
        std::vector<std::unique_ptr<tree_node_<T>>> child_nodes_;
    };

    /**
     * @brief Iterator base to traverse through the tree
     *
     * The Iterator base is to build other types of iterator. It doesn't handle
     * any traversal logics; just store the information of the current iterator.
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

        iterator_base_(tree_node_<T> *node) : node_(node)
        {
            if (node->parent_node_ != nullptr)
            {
                for (auto curr
                     = std::begin(this->node_->parent_node_->child_nodes_);
                     curr != std::end(this->node_->parent_node_->child_nodes_);
                     ++curr)
                {
                    if ((*curr).get() == node)
                    {
                        this->idx_
                            = curr
                              - std::begin(
                                  this->node_->parent_node_->child_nodes_);
                    }

                    break;
                }

                this->siblings_
                    = this->node_->parent_node_->child_nodes_.size();
            }
        }

        reference_type operator*() const noexcept { return node_->value_; }

        pointer_type operator->() const noexcept { return &(node_->value_); }

        tree_node_<T> *node_         = nullptr;
        bool           skip_current_ = false;
        size_type      idx_          = 0;
        size_type      siblings_     = 0;
    };

    template <typename T>
    class pre_ordered_iterator : public iterator_base_<T>
    {
    public:
        pre_ordered_iterator() = default;

        pre_ordered_iterator(tree_node_<T> *node) : iterator_base_<T>(node) {}

        bool operator==(const pre_ordered_iterator<T> &iterator) const
        {
            return (this->node_ == iterator.node_);
        }

        bool operator!=(const pre_ordered_iterator<T> &iterator) const
        {
            return !(this->node_ == iterator.node_);
        }

        pre_ordered_iterator<T> &operator++()
        {
            // Always traverse down to the leaves
            if (!this->node_->child_nodes_.empty())
            {
                this->idx_      = 0;
                this->siblings_ = this->node_->child_nodes_.size();
                this->node_     = this->node_->child_nodes_[0].get();
            }

            // If there is no child node to traverse, go to siblings
            else if (this->idx_ < this->siblings_ - 1)
            {
                ++this->idx_;
                this->node_
                    = this->node_->parent_node_->child_nodes_[this->idx_].get();
            }

            // If there is no sibling node to traverse, traverse back to parent
            else if (this->node_->parent_node_->parent_node_ != nullptr)
            {
                // Keep track of the parent node of the current node
                // Then move to the next sibling of that parent node.

                std::size_t parent_idx = 0;
                for (auto curr = this->node_->parent_node_->parent_node_
                                     ->child_nodes_.begin();
                     curr
                     != this->node_->parent_node_->parent_node_->child_nodes_
                            .end();
                     ++curr, ++parent_idx)
                {
                    if ((*curr).get() == this->node_->parent_node_)
                        break;
                }

                // Reaching the end of the tree
                if (parent_idx
                    == this->node_->parent_node_->parent_node_->child_nodes_
                               .size()
                           - 1)
                {
                    this->node_ == nullptr;
                }

                else
                {
                    // Update states of the newly moved node
                    this->idx_      = parent_idx + 1;
                    this->siblings_ = this->node_->parent_node_->parent_node_
                                          ->child_nodes_.size();

                    this->node_ = this->node_->parent_node_->parent_node_
                                      ->child_nodes_[this->idx_]
                                      .get();
                }
            }
            else
            {
                this->idx_++;
                this->node_ = nullptr;
            }

            return *this;
        }

        pre_ordered_iterator<T> operator++(int)
        {
            pre_ordered_iterator<T> copy = *this;
            ++(*this);
            return copy;
        }

        pre_ordered_iterator<T> &operator+=(std::size_t num)
        {
            while (num > 0)
            {
                ++(*this);
                --num;
            }

            return *this;
        }
    };

    /**
     * @brief The core implementation for tree data structure
     *
     * This implementation is supposed to support general tree (n-ary tree). It
     * can work with other STL algorithms and containers based on the iterators
     * as well as provide modifier, access and operational methods to work by
     * itself.
     *
     * For simplicity, tree will use the pre-ordered traversal technique to
     * support iterator.
     */
    template <typename T>
    class tree
    {
    public:
        using value_type      = T;
        using reference_type  = T &;
        using pointer_type    = T *;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using node          = tree_node_<T>;
        using iterator_base = iterator_base_<T>;

        /**
         * @brief Default constructor. Creates an empty tree.
         */
        tree() = default;

        /**
         * @brief Copy constructor. Creates a tree with the copied value.
         *
         * @param value Value to intialize the tree
         */
        tree(const reference_type value) : root_(std::make_unique<node>(value))
        {
        }

        /**
         * @brief Move constructor. Creates a tree with the moved value.
         *
         * @param value Value to initialize the tree.
         */
        tree(T &&value) : root_(std::make_unique<node>(std::move(value))) {}

        /**
         * @brief Returns an iterator to the first element of the tree
         */
        pre_ordered_iterator<T> begin()
        {
            return pre_ordered_iterator<T>(this->root_.get());
        }

        /**
         * @brief Returns an iterator the end element of the tree
         *
         */
        pre_ordered_iterator<T> end() { return pre_ordered_iterator<T>(); }

        /**
         * @brief Appends a new child after the node pointed to by pos
         *
         * @param pos iterator after which the content will be appended.
         * @param value Copied value to insert
         */
        template <typename Iter>
        Iter append(Iter pos, const reference_type value)
        {
            return Iter(append_after_(pos, value));
        }

        /**
         * @brief Appends a remove reference child after the node pointed to by
         * pos
         *
         * @param pos iterator after which the content will be appended
         * @param value remove reference value
         */
        template <typename Iter>
        Iter append(Iter pos, const T &&value)
        {
            return Iter(append_after_(pos, std::move(value)));
        }

        /**
         * @brief Insert a copy value after a node pointed to by pos as a
         * sibling
         *
         * @param pos iterator after which the content will be inserted
         * @param value  copy value to insert
         */
        template <typename Iter>
        Iter insert(Iter pos, const reference_type value)
        {
            return Iter(insert_after_(pos, value));
        }

        /**
         * @brief Insert a remove referenced value after a node pointed to by
         * pos as a sibling
         *
         * @param pos  iterator after which teh content will inserted
         * @param value  remove reference to insert
         */
        template <typename Iter>
        Iter insert(Iter pos, const T &&value)
        {
            return Iter(insert_after_(pos, value));
        }

        /**
         * @brief Computes the total number of nodes in the tree
         */
        size_type size() const noexcept { return this->root_->size(); }

        /**
         * @brief Computes the longest distance from one node to its leaves
         */
        size_type height() const noexcept { return this->root_->height(); }

        /**
         * @brief Computes the distance from one node to its root
         */
        size_type depth() const noexcept { return this->root_->depth(); }

        /**
         * @brief Computes the total number of leaves in the tree
         */
        size_type breadth() const noexcept { return this->root_->breadth(); }

    private:
        std::unique_ptr<node> root_ = nullptr;

        template <typename Iter, typename... Args>
        node *append_after_(Iter pos, Args &&...args)
        {
            auto new_node = std::make_unique<node>(std::forward<Args>(args)...);

            new_node->parent_node_ = pos.node_;

            pos.node_->child_nodes_.push_back(std::move(new_node));

            // Appending the new node to the back, it always stays there.
            return pos.node_->child_nodes_[pos.node_->child_nodes_.size() - 1]
                .get();
        }

        template <typename Iter, typename... Args>
        node *insert_after_(Iter pos, Args &&...args)
        {
            auto new_node = std::make_unique<node>(std::forward<Args>(args)...);

            new_node->parent_node_ = pos.node_->parent_node_;

            auto insert_iterator
                = std::begin(pos.node_->parent_node_->child_nodes_) + pos.idx_;
            pos.node_->parent_node_->child_nodes_.insert(insert_iterator + 1,
                                                         std::move(new_node));

            return pos.node_->parent_node_->child_nodes_[pos.idx_ + 1].get();
        }
    };
} // namespace opendsa
