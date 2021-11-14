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
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <vector>

namespace opendsa
{

    /**
     * @brief A standard tree container whose node has at most two child
     * nodes and maintains its parent node if not a not root node.
     *
     * @tparam T Type of the elements in the tree
     *
     * The standard binary tree has a private class node_ to represent
     * the inside nodes. For tree, it will works with iterator classes
     * provided below, not the nodes directly. For nodes, it should only
     * work with "node_" class.
     */
    template <typename T>
    class binary_tree
    {
    private:
        /**
         * @brief A simple node that owns two child nodes and maintains
         * the parent node for utility.
         *
         * @tparam T Type of the element.
         */
        template <typename U>
        struct node_
        {
            U data_;

            node_<U> *parent_;

            std::unique_ptr<node_<U>> left_;
            std::unique_ptr<node_<U>> right_;

            /**
             * @brief Default constructor. Creates an empty binary tree node.
             */
            constexpr node_()
                : parent_(nullptr), left_(nullptr), right_(nullptr)
            {
            }

            /**
             * @brief Creates a binary tree node with specified value.
             *
             * @param data Data to create a node
             */
            constexpr node_(U data)
                : data_(data), parent_(nullptr), left_(nullptr), right_(nullptr)
            {
            }

            /**
             * @brief Move constructor. Creates a binary tree node with
             * specified value.
             *
             * @param data Data to create a node
             */
            constexpr node_(U &&data)
                : data_(std::move(data)), parent_(nullptr), left_(nullptr),
                  right_(nullptr)
            {
            }

            /**
             * @brief Copy constructor. Creates a binary tree with copied binary
             * tree.
             *
             * @param base Copied binary tree to create a tree
             */
            node_(const node_<U> *&base)
            {
                if (base != nullptr)
                {
                    this->data_ = base->data_;

                    copy_(this->left_, base->left_);
                    copy_(this->right_, base->right_);
                }
            }

            /**
             * @brief Move constructor. Move an existing tree to a binary tree.
             *
             * @param base Moved binary tree to create a tree.
             */
            node_(node_<U> *&&base)
            {
                if (base != nullptr)
                {
                    this->data_ = base->data_;

                    this->left_  = std::move(base->left_);
                    this->right_ = std::move(base->right_);
                }
            }

        private:
            void copy_(std::unique_ptr<node_ *> &      dest,
                       const std::unique_ptr<node_ *> &src)
            {
                if (src == nullptr)
                    dest = nullptr;
                else
                {
                    dest          = std::make_unique<node_ *>(src->data_);
                    dest->parent_ = src->parent_;

                    copy_(dest->left_, src->left_);
                    copy_(dest->right_, src->right_);
                }
            }
        };

        template <typename U>
        struct iterator_
        {
            using value_type     = U;
            using pointer_type   = U *;
            using reference_type = U &;

            using size_type         = std::size_t;
            using difference_type   = std::ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            constexpr iterator_() : n_(nullptr) {}

            constexpr iterator_(node_<U> *&node) : n_(node) {}

            constexpr iterator_(const iterator_<U> &base)
            {
                this->n_ = base.n_;
            }

            /**
             * @brief Gets the value of the iterator.
             */
            reference_type operator*() const { return this->n_->data_; }

            /**
             * @brief Gets the address of the value of the iterator.
             */
            pointer_type operator->() const { return &(this->n_->data_); }

            /**
             * @brief Compares two iterators for range-based and stuff
             */
            bool operator==(const iterator_ &other)
            {
                return (this->n_ == other.n_);
            }

            /**
             * @brief Compares two iterators for range-based and stuff
             */
            bool operator!=(const iterator_ &other)
            {
                return (this->n_ != other.n_);
            }

            node_<U> *n_;
        };

    public:
        template <typename U>
        class inorder_iterator : public iterator_<U>
        {
        public:
            constexpr inorder_iterator() = default;

            constexpr inorder_iterator(node_<U> *n) { this->n_ = n; }

            constexpr inorder_iterator(const iterator_<U> &base)
            {
                this->n_ = base.n_;
            }

            /**
             * @brief Prefix inorder traversal incrementation
             */
            inorder_iterator &operator++()
            {
                if (this->n_->right_)
                {
                    auto successor = this->n_->right_.get();

                    while (successor && successor->left_)
                    {
                        successor = successor->left_.get();
                    }

                    this->n_ = successor;
                }
                else
                {
                    auto parent = this->n_->parent_;

                    if (parent == nullptr)
                        this->n_ = nullptr;

                    else if (this->n_ == parent->left_.get())
                        this->n_ = parent;
                    else
                    {
                        while (parent && this->n_ == parent->right_.get())
                        {
                            this->n_ = parent;
                            parent   = parent->parent_;
                        }

                        this->n_ = parent;
                    }
                }

                return *this;
            }

            inorder_iterator operator++(int)
            {
                inorder_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            inorder_iterator &operator--()
            {
                if (!this->n_->left_ && !this->n_->right_)
                {
                    if (this->n_ == this->n_->parent_->right_.get())
                    {
                        this->n_ = this->n_->parent_;
                    }

                    else
                    {
                        auto parent = this->n_->parent_;

                        while (parent && this->n_ == parent->left_.get())
                        {
                            this->n_ = parent;
                            parent   = parent->parent_;
                        }

                        if (parent == nullptr)
                            this->n_ = nullptr;
                        else
                        {
                            this->n_ = parent;
                        }
                    }
                }
                else if (this->n_->left_)
                {
                    auto predecessor = this->n_->left_.get();

                    while (predecessor->right_.get())
                    {
                        predecessor = predecessor->right_.get();
                    }

                    this->n_ = predecessor;
                }

                return *this;
            }

            inorder_iterator operator--(int)
            {
                inorder_iterator tmp = *this;
                --(*this);

                return tmp;
            }
        };

        template <typename U>
        class preorder_iterator : public iterator_<U>
        {
        public:
            constexpr preorder_iterator() = default;

            constexpr preorder_iterator(node_<U> *n) { this->n_ = n; }

            constexpr preorder_iterator(const iterator_<U> &base)
            {
                this->n_ = base.n_;
            }

            /**
             * @brief Prefix preorder traversal incrementation
             */
            preorder_iterator &operator++()
            {
                // Go to left node if available
                if (this->n_->left_)
                    this->n_ = this->n_->left_.get();
                // If not, go to the right node if available
                else if (this->n_->right_)
                    this->n_ = this->n_->right_.get();
                // Otherwise, this is a leaf node
                else if (!this->n_->right_ && !this->n_->left_)
                {
                    // Check if this is a left child node
                    if (this->n_ == this->n_->parent_->left_.get())
                    {
                        auto parent = this->n_->parent_;

                        // Ignore already-visited nodes until there
                        // is a node with a right child node
                        while (parent && !parent->right_)
                        {
                            parent = parent->parent_;
                        }

                        // Traverse to root node
                        if (parent == nullptr)
                            this->n_ = nullptr;
                        else
                            this->n_ = parent->right_.get();
                    }
                    else
                    {
                        auto parent = this->n_->parent_;

                        // Escape the the subtree
                        while (parent && this->n_ == parent->right_.get())
                        {
                            this->n_ = parent;
                            parent   = parent->parent_;
                        }

                        // Convert back to the case ignore already-visited
                        // nodes.
                        while (parent && !parent->right_)
                        {
                            parent = parent->parent_;
                        }

                        if (parent == nullptr)
                            this->n_ = nullptr;
                        else
                            this->n_ = parent->right_.get();
                    }
                }

                return *this;
            }

            preorder_iterator operator++(int)
            {
                preorder_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            preorder_iterator &operator--()
            {
                if (!this->n_->left_ && !this->n_->right_)
                {
                    if (!this->n_->parent_)
                        this->n_ = nullptr;
                    else if (this->n_ == this->n_->parent_->right_.get())
                    {
                        auto parent = this->n_->parent;

                        while (parent && this->n_ == parent->right_.get())
                        {
                            this->n_ = parent;

                            if (this->n_->left_)
                                this->n_ = this->n_->left_.get();
                            else
                                parent = parent->parent_;
                        }

                        if (parent == nullptr)
                            this->n_ = nullptr;
                    }
                    else
                    {
                        auto parent = this->n_->parent_;

                        while (parent && this->n_ == parent->left_.get())
                        {
                            this->n_ = parent;
                            parent   = parent->parent_;
                        }

                        if (parent == nullptr)
                            this->n_ = nullptr;
                        else
                            this->n_ = parent->left_.get();
                    }
                }

                else if (this->n_->right_)
                    this->n_ = this->n_->right_.get();

                return *this;
            }

            preorder_iterator operator--(int)
            {
                preorder_iterator tmp = *this;
                --(*this);

                return tmp;
            }
        };

        template <typename U>
        class postorder_iterator : public iterator_<U>
        {
        public:
            constexpr postorder_iterator() = default;

            constexpr postorder_iterator(node_<U> *n) { this->n_ = n; }

            constexpr postorder_iterator(const iterator_<U> &base)
            {
                this->n_ = base.n_;
            }

            /**
             * @brief Prefix postorder traversal incrementation
             */
            postorder_iterator &operator++()
            {
                auto parent = this->n_->parent_;

                if (parent == nullptr)
                    this->n_ = parent;
                else if (this->n_ == parent->right_.get())
                {
                    this->n_ = parent;
                }
                else
                {
                    if (parent->right_)
                    {
                        auto successor = parent->right_.get();

                        while (successor && successor->left_)
                            successor = successor->left_.get();

                        this->n_ = successor;
                    }
                    else
                    {
                        this->n_ = parent;
                    }
                }

                return *this;
            }

            postorder_iterator operator++(int)
            {
                postorder_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            postorder_iterator &operator--()
            {
                if (this->n_->parent_ == nullptr)
                    this->n_ = nullptr;
                else
                {
                    if (this->n_ == this->n_->parent_->right_.get())
                    {
                        auto predecessor = this->n_->parent_->left_.get();

                        while (predecessor && predecessor->right_)
                            predecessor = predecessor->right_.get();

                        if (predecessor == nullptr)
                            this->n_ = nullptr;
                        else
                            this->n_ = predecessor;
                    }
                    else
                        this->n_ = this->n_->parent_;
                }

                return *this;
            }

            postorder_iterator operator--(int)
            {
                postorder_iterator tmp = *this;
                --(*this);

                return tmp;
            }
        };

        template <typename U>
        class levelorder_iterator : public iterator_<U>
        {
        public:
            constexpr levelorder_iterator() = default;

            constexpr levelorder_iterator(node_<U> *n)
            {
                this->n_ = n;

                if (n->left_)
                    this->queue_.push_back(n->left_.get());

                if (n->right_)
                    this->queue_.push_back(n->right_.get());
            }

            constexpr levelorder_iterator(const iterator_<U> &base)
            {
                this->n_ = base.n_;

                if (base.n_->left_)
                    this->queue_.push_back(base.n_->left_.get());

                if (base.n_->right_)
                    this->queue_.push_back(base.n_->right_.get());
            }

            /**
             * @brief Level order iterator incrementation
             */
            levelorder_iterator &operator++()
            {
                if (this->queue_.empty())
                    this->n_ = nullptr;
                else
                {
                    this->stack_.push_back(this->n_);
                    this->n_ = this->queue_.front();
                    this->queue_.pop_front();

                    if (this->n_->left_)
                        this->queue_.push_back(this->n_->left_.get());

                    if (this->n_->right_)
                        this->queue_.push_back(this->n_->right_.get());
                }

                return *this;
            }

            levelorder_iterator operator++(int)
            {
                levelorder_iterator tmp = *this;
                ++(*this);

                return tmp;
            }

            levelorder_iterator &operator--()
            {
                if (!stack_.empty())
                    this->n_ = nullptr;
                else
                {
                    this->n_ = this->stack_.back();
                    this->queue_.push_front(this->n_);
                    this->stack_.pop_back();

                    if (this->n_->right_.get() == this->queue_.back())
                    {
                        this->queue_.pop_back();
                    }

                    if (this->n_->left_.get() == this->queue_.back())
                    {
                        this->queue_.pop_back();
                    }
                }

                return *this;
            }

            levelorder_iterator operator--(int)
            {
                levelorder_iterator tmp = *this;
                ++(*this);

                return tmp;
            }

        private:
            std::deque<node_<U> *> queue_;
            std::deque<node_<U> *> stack_;
        };

        /**
         * @brief Default constructor. Constructs an empty binary tree.
         */
        constexpr binary_tree() : root_(nullptr) {}

        /**
         * @brief Constructs a binary tree with root.
         *
         * @param data Data to create the root.
         */
        constexpr explicit binary_tree(T data)
        {
            this->root_ = std::make_unique<node_<T>>(data);
        }

        /**
         * @brief Copy constructor. Initializes a new binary tree from a
         * copied version of another binary tree.
         *
         * @param other Another copied binary tree to create a new tree.
         */
        constexpr binary_tree(const binary_tree<T> &other)
        {
            if (other.root_ != nullptr)
                copy_(this->root_, other.root_);
        }

        /**
         * @brief Move constructor. Initializes a new binary tree by moving
         * ownership from another tree.
         *
         * @param other Rvalue-binary tree to create a new tree.
         *
         */
        constexpr binary_tree(binary_tree<T> &&other)
        {
            if (other.root_ != nullptr)
                this->root_ = std::move(other->root_);
        }

        /**
         * @brief Destroys the binary tree object.
         */
        ~binary_tree() { this->root_.reset(); }

        binary_tree &operator=(const binary_tree<T> &other)
        {
            if (this != &other || other.root_ != nullptr)
            {
                copy_(this->root_, other.root_);
            }
        }

        inorder_iterator<T> inorder_begin() const
        {
            node_<T> *successor = this->root_.get();

            while (successor->left_)
            {
                successor = successor->left_.get();
            }

            return inorder_iterator(successor);
        }

        inorder_iterator<T> inorder_end() const
        {
            return inorder_iterator<T>();
        }

        preorder_iterator<T> preorder_begin() const
        {
            return preorder_iterator<T>(this->root_.get());
        }

        preorder_iterator<T> preorder_end() const
        {
            return preorder_iterator<T>();
        }

        postorder_iterator<T> postorder_begin() const
        {
            node_<T> *successor = this->root_.get();

            while (successor->left_ || successor->right_)
            {
                if (successor->left_)
                    successor = successor->left_.get();

                else
                    successor = successor->right_.get();
            }

            return postorder_iterator(successor);
        }

        postorder_iterator<T> postorder_end() const
        {
            return postorder_iterator<T>();
        }

        levelorder_iterator<T> levelorder_begin() const
        {
            return levelorder_iterator<T>(this->root_.get());
        }

        levelorder_iterator<T> levelorder_end() const
        {
            return levelorder_iterator<T>();
        }

        inorder_iterator<T> begin() const { return inorder_begin(); }

        inorder_iterator<T> end() const { return inorder_end(); }

        /**
         * @brief Displays the binary tree in in-order fashion.
         */
        void inorder() const { std::cout << *this; }

        /**
         * @brief Displays the binary tree in pre-order fashion.
         *
         */
        void preorder() const
        {

            for (auto curr = preorder_begin(); curr != preorder_end(); ++curr)
            {
                std::cout << *curr << ",";
            }
            std::cout << std::endl;
        }

        /**
         * @brief Displays the binary tree in post-order fashion.
         */
        void postorder() const
        {

            for (auto curr = postorder_begin(); curr != postorder_end(); ++curr)
            {
                std::cout << *curr << ",";
            }
            std::cout << std::endl;
        }

        /**
         * @brief Displays the binary tree in level-order fashion
         */
        void levelorder() const
        {

            for (auto curr = levelorder_begin(); curr != levelorder_end();
                 ++curr)
            {
                std::cout << *curr << ",";
            }
            std::cout << std::endl;
        }

        node_<T> *root() const { return this->root_.get(); }

        template <typename Iter = iterator_<T>>
        Iter insert_left(Iter pos, const T &value)
        {
            insert_node_(pos.n_->left_, pos.n_, value);

            return Iter(pos.n_->left_.get());
        }

        template <typename Iter = iterator_<T>>
        Iter insert_right(Iter pos, const T &value)
        {
            insert_node_(pos.n_->right_, pos.n_, value);

            return Iter(pos.n_->right_.get());
        }

        template <typename Iter = iterator_<T>>
        Iter insert_left_subtree(Iter pos, node_<T> *node)
        {
            insert_subtree_(pos.n_->left_, pos.n_, node);

            return Iter(pos.n_->left_.get());
        }

        template <typename Iter = iterator_<T>>
        Iter insert_right_subtree(Iter pos, node_<T> *node)
        {
            insert_subtree_(pos.n_->right_, pos.n_, node);

            return Iter(pos.n_->right_.get());
        }

        friend std::ostream &operator<<(std::ostream &        out,
                                        const binary_tree<T> &tree)
        {
            for (auto &e : tree)
                out << e << ",";

            out << std::endl;
        }

    private:
        std::unique_ptr<node_<T>> root_;

        void copy_(std::unique_ptr<node_<T>> &      dest,
                   const std::unique_ptr<node_<T>> &src)
        {
            // Use copy_ to create a new node
            if (src == nullptr)
                return;

            dest          = std::make_unique<node_<T>>(src->data_);
            dest->parent_ = src->parent_;

            copy_(dest->left_, src->left_);
            copy_(dest->right_, src->right_);
        }

        template <typename... Args>
        void insert_node_(std::unique_ptr<node_<T>> &node, node_<T> *&parent,
                          Args &&...args)
        {
            if (node != nullptr)
            {
                std::cerr << "There is a node existing at this position, "
                             "insert aborted!\n";
                return;
            }

            node = std::make_unique<node_<T>>(std::forward<Args>(args)...);
            node->parent_ = parent;
        }

        void insert_subtree_(std::unique_ptr<node_<T>> &node, node_<T> *&parent,
                             node_<T> *other)
        {
            if (node != nullptr)
            {
                std::cerr << "There is a node existing at this position, "
                             "insert aborted!\n";
                return;
            }

            node          = std::make_unique<node_<T>>(other->data_);
            node->parent_ = parent;

            copy_(node->left_, other->left_);
            copy_(node->right_, other->right_);
        }
    };

    template <typename T>
    std::vector<T> inorder_flatten(const binary_tree<T> &tree)
    {
        std::vector<T> v;

        for (auto &e : tree)
            v.push_back(e);

        return v;
    }
} // namespace opendsa
