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

/**
 * @brief A basic binary tree node.
 *
 * The purpose of this base struct is to provide basic methods t work with this
 * tree regardless of the type. _tree_node_base doesn't have any restriction and
 * all of the methods are public.
 */
template <typename _Tp>
struct _Bi_tree_node_base
{
    using base_ptr       = _Bi_tree_node_base<_Tp> *;
    using const_base_ptr = const _Bi_tree_node_base<_Tp> *;

    base_ptr _parent;
    base_ptr _left;
    base_ptr _right;
    _Tp _val;

    /**
     * @brief Returns the leftmost child node of the given tree.
     */
    static base_ptr
    _leftmost(base_ptr _x) noexcept
    {
        while (_x->_left != nullptr)
            _x = _x->_left;
        return _x;
    }

    /**
     * @brief  Returns the leftmost child node of the given tree.
     */
    static const_base_ptr
    _leftmost(const_base_ptr _x) noexcept
    {
        return _Bi_tree_node_base<_Tp>::_leftmost(const_cast<base_ptr>(_x));
    }

    /**
     * @brief Returns the rightmost child node of the given tree.
     */
    static base_ptr
    _rightmost(base_ptr _x) noexcept
    {
        while (_x->_right != nullptr)
            _x = _x->_right;
        return _x;
    }

    /**
     * @brief Returns the rightmost child node of the given tree.
     */
    static const_base_ptr
    _rightmost(const_base_ptr _x) noexcept
    {
        return _Bi_tree_node_base<_Tp>::_rightmost(const_cast<base_ptr>(_x));
    }

    _Tp *
    _valptr()
    {
        return std::addressof(_val);
    }

    const _Tp *
    _valptr() const
    {
        return std::addressof(_val);
    }
};

template <typename _Tp>
struct _Bi_tree_iterator
{
    using value_type = _Tp;
    using reference  = _Tp &;
    using pointer    = _Tp *;

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    using self      = _Bi_tree_iterator<_Tp>;
    using node_type = _Bi_tree_node_base<_Tp>;
    using base_ptr  = _Bi_tree_node_base<_Tp>::base_ptr;

    base_ptr _node;

    _Bi_tree_iterator() noexcept : _node() { }

    explicit _Bi_tree_iterator(base_ptr _x) noexcept : _node(_x) { }

    reference
    operator*() const noexcept
    {
        return *(_node->_valptr());
    }

    pointer
    operator->() const noexcept
    {
        return _node->_valptr();
    }
};

template <typename _Tp>
struct _Bi_tree_inorder_iterator : public _Bi_tree_iterator<_Tp>
{
    using base_ptr  = _Bi_tree_iterator<_Tp>::base_ptr;
    using node_type = _Bi_tree_iterator<_Tp>::node_type;

private:
    base_ptr
    _increment(base_ptr _x) noexcept
    {
        if (_x->_right != nullptr)
        {
            _x = _x->_right;
            _x = node_type::_leftmost(_x);
        }
        else
        {
            base_ptr _x_parent = _x->_parent;
            while (_x == _x_parent->_right)
            {
                _x        = _x_parent;
                _x_parent = _x_parent->_parent;
            }

            if (_x->_right != _x_parent)
                _x = _x_parent;
        }

        return _x;
    }
};

} // namespace opendsa

#endif /* __OPENDSA_TREE_H */
