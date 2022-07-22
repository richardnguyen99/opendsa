/**
 * @file tree.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief Tree data structures
 * @version 0.1
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_TREE_H
#define __OPENDSA_TREE_H 1

#include <cstddef>
#include <memory>

namespace opendsa
{
    struct _tree_node_base
    {
        using base_ptr       = _tree_node_base *;
        using const_base_ptr = const _tree_node_base *;

        base_ptr _parent = nullptr;
        base_ptr _left   = nullptr;
        base_ptr _right  = nullptr;

        static base_ptr _max_left(base_ptr _x) noexcept
        {
            while (_x->_left != nullptr)
                _x = _x->_left;
            return _x;
        }

        static const_base_ptr _max_left(const_base_ptr _x) noexcept
        {
            while (_x->_left != nullptr)
                _x = _x->_left;
            return _x;
        }

        static base_ptr _max_right(base_ptr _x) noexcept
        {
            while (_x->_right != nullptr)
                _x = _x->_right;
            return _x;
        }

        static const_base_ptr _max_right(const_base_ptr _x) noexcept
        {
            while (_x->_right != nullptr)
                _x = _x->_right;
            return _x;
        }

        static base_ptr _increment(base_ptr _x)
        {
            if (_x->_right)
            {
                _x = _x->_right;
                _x = _max_left(_x);
            }
            else
            {
                base_ptr _x_parent = _x->_parent;
                while (_x_parent && _x == _x_parent->_right)
                {
                    _x        = _x_parent;
                    _x_parent = _x_parent->_parent;
                }

                if (_x->_right != _x_parent)
                    _x = _x_parent;
            }

            return _x;
        }

        static const_base_ptr _increment(const_base_ptr _x)
        {
            return _increment(const_cast<base_ptr>(_x));
        }

        static base_ptr _decrement(base_ptr _x)
        {
            if (_x->_left)
            {
                _x = _x->_left;
                _x = _max_right(_x->_right);
            }
            else
            {
                base_ptr _x_parent = _x->_parent;
                while (_x == _x_parent->_left)
                {
                    _x        = _x_parent;
                    _x_parent = _x_parent->_parent;
                }

                if (_x->_left != _x_parent)
                    _x = _x_parent;
            }

            return _x;
        }

        static const_base_ptr _decrement(const_base_ptr _x)
        {
            return _decrement(const_cast<base_ptr>(_x));
        }
    };

    template <typename _Tp>
    struct _tree_node : public _tree_node_base
    {
        using link_type = _tree_node<_Tp> *;

        _Tp _storage;

        _Tp *getptr()
        {
            return std::addressof(_storage);
        }

        const _Tp *getptr() const
        {
            return std::addressof(_storage);
        }
    };

    template <typename _Tp>
    struct tree_iterator
    {
        using value_type = _Tp;
        using reference  = _Tp &;
        using pointer    = _Tp *;

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using size_type         = std::size_t;

        using self      = tree_iterator<_Tp>;
        using base      = _tree_node_base;
        using base_ptr  = _tree_node_base::base_ptr;
        using link_type = _tree_node<_Tp> *;

        tree_iterator() noexcept : _node() {}

        explicit tree_iterator(base_ptr _x) noexcept : _node(_x) {}

        reference operator*() const noexcept
        {
            return *static_cast<link_type>(_node)->getptr();
        }

        pointer operator->() const noexcept
        {
            return static_cast<link_type>(_node)->getptr();
        }

        self &operator++() noexcept
        {
            _node = base::_increment(_node);
            return *this;
        }

        self operator++(int) noexcept
        {
            self tmp = *this;
            _node    = base::_increment(_node);
            return tmp;
        }

        base_ptr _node;
    };

    /**
     * @brief A simple binary_tree
     *
     * This is a base class for a binary tree data structure. It provides basic
     * methods to work with data held in the tree. There is no restriction or
     * constraint on how those data is implemented.
     */
    template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
    struct binary_tree
    {
    protected:
    };

} // namespace opendsa

#endif /* __OPENDSA_TREE_H */
