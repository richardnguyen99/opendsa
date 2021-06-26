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

#include <memory>
#include <vector>
#include <queue>

namespace opendsa
{
    /**
     * @brief Basic, general basic tree implementation
     * 
     * This class represents a node of a tree. It holds 
     * a value to be its representative. The node may 
     * also contain a parent pointer and many unique
     * child pointers.
     * 
     * @tparam T 
     */
    template <typename T>
    class tree_node
    {
    public:
        tree_node(const T &value) : value_{value} {}

        /**
         * @brief Count all elements in the tree
         */
        std::size_t size() const
        {
            std::size_t size = 1;

            for (const std::unique_ptr<tree_node<T>> &child : children_)
                size += child->size();

            return size;
        }

        /**
         * @brief Return the longest distance from root to leaf
         */
        std::size_t height() const
        {
            std::size_t height = 0;

            if (!children_.empty())
            {
                // Store heights of sub trees to find the tallest
                std::vector<std::size_t> sub_heights;

                // Recursively find sub heights
                std::transform(std::cbegin(children_), std::cend(children_), std::back_inserter(sub_heights), [](const std::unique_ptr<tree_node<T>> &child)
                               { return child->height(); });

                // Find the highest tree
                height = *(std::max_element(std::cbegin(sub_heights), std::cend(sub_heights))) + 1;
            }

            return height;
        }

        /**
         * @brief Return the distance to the root
         */
        std::size_t depth() const
        {
            std::size_t depth = 0;

            // Traverse back to its ancestor nodes until the root node.
            if (parent_ptr_ != nullptr)
            {
                depth = parent_ptr_->depth() + 1;
            }

            return depth;
        }

        /**
         * @brief Get the max number of children of any sub node
         */
        std::size_t degree() const
        {
            std::size_t degree = 0;

            if (!children_.empty())
            {
                // Check degrees of sub node
                std::vector<std::size_t> sub_degrees;

                std::transform(std::cbegin(children_), std::cend(children_), std::back_inserter(sub_degrees), [](const std::unique_ptr<tree_node<T>> &child)
                               { return child->degree(); });

                std::size_t max_sub_degree = *(std::max_element(std::cbegin(sub_degrees), std::cend(sub_degrees)));

                degree = std::max(children_.size(), max_sub_degree);
            }

            return degree;
        }

        /**
         * @brief Get the number of leaves of a tree
         */
        std::size_t breadth() const
        {
            // No children means the node is leaf
            if (children_.empty())
            {
                return 1;
            }

            std::size_t sub_leaves = 0;
            for (const std::unique_ptr<tree_node<T>> &child : children_)
            {
                sub_leaves += child->breadth();
            }

            return sub_leaves;
        }

        /**
         * @brief Insert a unique element to the tree
         */
        void insert(std::unique_ptr<tree_node<T>> child)
        {
            child->parent_ptr_ = this;
            children_.push_back(std::move(child));
        }

        /**
         * @brief Get the root value of the tree
         */
        T &root() const
        {
            return value_;
        }

    private:
        T value_;

        // Parent pointer cannnot be unique as there may be
        // more than one pointer pointing to it.
        tree_node<T> *parent_ptr_ = nullptr;
        std::vector<std::unique_ptr<tree_node<T>>> children_;
    };

    /**
     * @brief Binary tree implementation
     */
    template <typename T>
    struct binary_tree_node
    {
        binary_tree_node(const T &value) : value_{value} {}

        T value_;
        binary_tree_node<T> *parent_ptr = nullptr;
        std::unique_ptr<binary_tree_node<T>> left_uptr_ = nullptr;
        std::unique_ptr<binary_tree_node<T>> right_uptr_ = nullptr;
    };

    /**
     * @brief Binary search tree with specified comparator
     */
    template <typename T, typename Comparator>
    class binary_search_tree
    {
    public:
        binary_search_tree(const T &value, Comparator cmp)
        {
            root_uptr_ = std::make_unique<binary_tree_node<T>>(value);
            comparator_ = cmp;
        }

        binary_tree_node<T> &root() const
        {
            return *root_uptr_;
        }

        void insert(const T &value)
        {
            auto node = std::make_unqiue<binary_tree_node<T>>(value);

            if (root_uptr_ == nullptr)
            {
                root_uptr_ = std::move(node);
            }
            else
            {
                insert_subtree(std::move(node), root_uptr_.get());
            }
        }

    private:
        std::unique_ptr<binary_tree_node<T>> root_uptr_ = nullptr;

        Comparator comparator_;

        void insert_subtree(std::unique_ptr<binary_tree_node<T>> node, binary_tree_node<T> *root_ptr)
        {
            if (comparator_(node->value_, root_ptr->value_))
            {
                if (root_ptr->left_uptr_ == nullptr)
                {
                    root_ptr->left_uptr_ = std::move(node);
                }
                else
                {
                    insert_subtree(std::move(node), root_uptr_->left_uptr_.get());
                }
            }
            else
            {
                if (root_ptr->right_uptr_ == nullptr)
                {
                    root_ptr->right_uptr_ = std::move(node);
                }
                else
                {
                    insnert_subtree(std::move(node), root_uptr_->right_uptr_.get());
                }
            }
        }
    };
}