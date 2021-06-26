/**
 * @file tree_algorithm.h
 * @author Richard Nguyen
 * @brief Algorithm implementation for tree data structures
 * @version 0.1
 * @date 2021-06-25
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "tree.h"

namespace opendsa
{
    /**
     * @brief Get the maximum height of a binary tree
     */
    template <typename T>
    std::size_t binary_tree_height(const binary_tree_node<T> &node)
    {
        // Compute the height of the left sub tree
        std::size_t left_height = 0;

        if (node.left_uptr_ != nullptr)
        {
            left_height = binary_tree_height(*(node.left_uptr_)) + 1;
        }

        // Compute the height of the righ sub tree
        std::size_t right_height = 0;

        if (node.right_uptr_ != nullptr)
        {
            right_height = binary_tree_height(*(node.right_uptr_)) + 1;
        }

        // Return the larger height
        return std::max(left_height, right_height);
    }

    /**
     * @brief Traverse through a binary tree using depth-first-search technique
     */
    template <typename T, typename Visitor>
    void depth_first_search(const binary_tree_node<T> &node, Visitor visitor)
    {
        // Traverse the left most sub trees
        if (node.left_uptr_ != nullptr)
        {
            depth_first_search(*(node.left_uptr_), visitor);
        }

        // Traverse the right most sub trees after traversing the left most trees
        if (node.right_uptr_ != nullptr)
        {
            depth_first_search(*(node.right_uptr_), visitor);
        }

        visitor(node.value_);
    }

    /**
     * @brief Traverse through a binary tree using breadth-first-search technique
     */
    template <typename T, typename Visitor>
    void breadth_first_search(const binary_tree_node<T> &node, Visitor visitor)
    {
        // Support traversing from top to bottom
        std::queue<binary_tree_node<T> *> remaining_nodes;
        remaining_nodes.push(&node);

        while (!remaining_nodes.empty())
        {
            // After traversing through one node, remove it from the queue
            const binary_tree_node<T> &front = *(remaining_nodes.front());
            remaining_nodes.pop();

            if (front.left_uptr_ != nullptr)
            {
                remaining_nodes.push(front.left_uptr_.get());
            }

            if (front.right_uptr_ != nullptr)
            {
                remaining_nodes.push(front.right_uptr_.get());
            }

            visitor(front.value_);
        }
    }

    /**
     * @brief Traverse through a binary tree using pre_order technique
     */
    template <typename T, typename Visitor>
    void pre_order_traverse(const binary_tree_node<T> &node, Visitor visitor)
    {
        // Visit the current node first before its children
        visitor(node.value_);

        if (node.left_uptr_ != nullptr)
        {
            pre_order_traverse(*(node.left_uptr_), visitor);
        }

        if (node.right_uptr_ != nullptr)
        {
            pre_order_traverse(*(node.right_uptr_), visitor);
        }
    }

    /**
     * @brief Traverse through a binary tree using in_order technique
     */
    template <typename T, typename Visitor>
    void in_order_traverse(const binary_tree_node<T> &node, Visitor visitor)
    {
        if (node.left_uptr_ != nullptr)
        {
            in_order_traverse(*(node.left_uptr_), visitor);
        }

        // Visit the left child, then the current node and finally the right child
        visitor(node.value_);

        if (node.right_uptr_ != nullptr)
        {
            in_order_traverse(*(node.right_uptr_), visitor);
        }
    }

    /**
     * @brief Traverse through a binary tree using post_order technique
     */
    template <typename T, typename Visitor>
    void post_order_traverse(const binary_tree_node<T> &node, Visitor visitor)
    {
        if (node.left_uptr_ != nullptr)
        {
            post_order_traverse(node.left_uptr_, visitor);
        }

        if (node.right_uptr_ != nullptr)
        {
            post_order_traverse(node.right_uptr_, visitor);
        }

        // Visit the left child first, then the right child and finally the current node
        visitor(node.value_);
    }
}