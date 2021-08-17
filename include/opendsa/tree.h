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
            // Continue to go down if there are child nodes available
            if (!this->node_->child_nodes_.empty())
            {
                this->node_ = this->node_->child_nodes[0].get();
                this->idx_  = 0;
                this->siblings_
                    = this->node_->parent_node_->child_nodes_.size();
            }
            else
            {
                if ((++this->idx_) < this->siblings_)
                {
                    this->node_
                        = this->node_->parent_node_->child_nodes_[this->idx_]
                              .get();
                }
                else
                {
                    auto next_node_index
                        = this->node_->parent_node_->parent_node_->child_nodes_
                              .begin()
                          - std::find(this->node_->parent_node_->parent_node_
                                          ->child_nodes_.begin(),
                                      this->node_->parent_node_->parent_node_
                                          ->child_nodes_.end(),
                                      this->node_->parent_node_)
                          + 1;

                    this->node_ = this->node_->parent_node_->parent_node_
                                      ->child_nodes_[next_node_index]
                                      .get();
                }
            }
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

        pre_ordered_iterator<T> begin()
        {
            return pre_ordered_iterator<T>(this->root_.get());
        }

        /**
         * @brief Appends a new child after the node pointed to by pos
         *
         * @param pos iterator after which the content will be appended.
         */
        template <typename Iter>
        Iter append(Iter pos, const reference_type value)
        {
            return Iter(append_after_(pos, value));
        }

        template <typename Iter>
        Iter append(Iter pos, const T &&value)
        {
            return Iter(append_after_(pos, std::move(value)));
        }

    private:
        std::unique_ptr<node> root_ = nullptr;

        template <typename Iter, typename... Args>
        node *append_after_(Iter pos, Args &&...args)
        {
            auto new_node = std::make_unique<node>(std::forward<Args>(args)...);

            new_node->parent_node_ = pos.node_;

            pos.node_->child_nodes_.push_back(std::move(new_node));

            // Appending the new node to the back, it always stay there.
            return pos.node_->child_nodes_[pos.node_->child_nodes_.size() - 1]
                .get();
        }
    };
} // namespace opendsa
