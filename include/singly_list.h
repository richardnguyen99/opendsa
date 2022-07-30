/**
 * @file list.h
 * @author (richard.ng0616@gmail.com)
 * @brief Header for singly-linked-list data structure
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __OPENDSA_SGL_LIST_H
#define __OPENDSA_SGL_LIST_H 1

#include <cstddef>
#include <forward_list>
#include <memory>

namespace opendsa
{
struct _Sgl_list_node_base
{
    _Sgl_list_node_base *_next = nullptr;

    _Sgl_list_node_base() = default;

    _Sgl_list_node_base(_Sgl_list_node_base &&_x) noexcept : _next(_x._next)
    {
        _x._next = nullptr;
    }

    // Suppress copy constructor so that every node is uniquely handled by
    // the previous node.
    _Sgl_list_node_base(const _Sgl_list_node_base &) = delete;

    // Suppress copy assignment so that every node is uniquely handled by
    // previous node.
    _Sgl_list_node_base &
    operator=(const _Sgl_list_node_base &) = delete;

    _Sgl_list_node_base &
    operator=(_Sgl_list_node_base &&_x)
    {
        this->_next = _x._next;
        _x._next    = nullptr;
        return *this;
    }
};

template <typename _Tp>
struct _Sgl_list_node : public _Sgl_list_node_base
{
    _Sgl_list_node() = default;

    _Tp _val;

    // valptr() returns the address of member _val. The purpose of this is
    // that while _val holds the actual object, not address, allocators can
    // construct a new object through valptr().
    // This also allows iterator class to perform dereferencing and pointer
    // access operations.

    _Tp *
    valptr() noexcept
    {
        return std::addressof(this->_val);
    }

    const _Tp *
    valptr() const noexcept
    {
        return std::addressof(this->_val);
    }
};

/**
 * @brief Used in singly_list as singly_list::iterator
 */
template <typename _Tp>
struct _Sgl_list_iterator
{
    using _self = _Sgl_list_iterator<_Tp>;

    using value_type      = _Tp;
    using reference       = _Tp &;
    using pointer         = _Tp *;
    using difference_type = std::ptrdiff_t;

    // Singly-linked-list is forward directional, which means it can only go
    // forward but cannot go backward. Therefore, only operator++() is
    // overloaded to make the iterator move forward and no operator--() is
    // overloaded to make the iterator move backward.
    using iterator_category = std::forward_iterator_tag;

    _Sgl_list_node_base *_node;

    _Sgl_list_iterator() noexcept : _node() { }

    explicit _Sgl_list_iterator(_Sgl_list_node_base *_n) noexcept
    : _node(_n) { }

    reference
    operator*() const noexcept
    {
        return *static_cast<_Sgl_list_node<value_type> *>(this->_node)
                    ->valptr();
    }

    pointer
    operator->() const noexcept
    {
        return static_cast<_Sgl_list_node<value_type> *>(this->_node)->valptr();
    }

    _self &
    operator++() noexcept
    {
        this->_node = this->_node->_next;
        return *this;
    }

    _self
    operator++(int) noexcept
    {
        _self tmp(*this);
        this->_node = this->_node->_next;
        return tmp;
    }

    bool
    operator==(const _self &_other) const noexcept
    {
        return this->_node == _other._node;
    }
};

/**
 * @brief Used in singly_list as singly_list::const_const_iterator
 */
template <typename _Tp>
struct _Sgl_list_const_iterator
{
    using _self = _Sgl_list_const_iterator<_Tp>;

    using value_type      = _Tp;
    using reference       = _Tp &;
    using pointer         = _Tp *;
    using difference_type = std::ptrdiff_t;

    // Singly-linked-list is forward directional, which means it can only go
    // forward but cannot go backward. Therefore, only operator++() is
    // overloaded to make the iterator move forward and no operator--() is
    // overloaded to make the iterator move backward.
    using iterator_category = std::forward_iterator_tag;

    _Sgl_list_node_base *_node;

    _Sgl_list_const_iterator() noexcept : _node() { }

    explicit _Sgl_list_const_iterator(_Sgl_list_node_base *_n) noexcept
    : _node(_n)
    {
    }

    reference
    operator*() const noexcept
    {
        return *static_cast<_Sgl_list_node<_Tp> *>(this->_node)->valptr();
    }

    pointer
    operator->() const noexcept
    {
        return static_cast<_Sgl_list_node<_Tp> *>(this->_node)->valptr();
    }

    _self &
    operator++() noexcept
    {
        this->_node = this->_node->_next;
        return *this;
    }

    _self
    operator++(int) noexcept
    {
        _self tmp(*this);
        this->_node = this->_node->_next;
        return tmp;
    }

    bool
    operator==(const _self &_other) const noexcept
    {
        return this->_node == _other._node;
    }
};

template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class singly_list
{
public:
private:
    using node_base      = _Sgl_list_node_base;
    using node           = _Sgl_list_node<_Tp>;
    using node_allocator = typename std::allocator_traits<
        _Alloc>::template rebind_alloc<_Sgl_list_node<_Tp>>;
    using node_traits = std::allocator_traits<node_allocator>;
    using type_traits = std::allocator_traits<_Alloc>;

public:
    using value_type      = _Tp;
    using pointer         = typename type_traits::pointer;
    using const_pointer   = typename type_traits::const_pointer;
    using reference       = _Tp &;
    using const_reference = const _Tp &;

    using iterator        = _Sgl_list_iterator<_Tp>;
    using const_iterator  = _Sgl_list_const_iterator<_Tp>;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type  = _Alloc;

    singly_list() : _header() { }

    explicit singly_list(size_type _n) : _header()
    {
        this->_default_initialize(_n);
    }

    singly_list(size_type _n, const value_type &_x)
    {
        this->_fill_initialize(_n, _x);
    }

    ~singly_list() { this->_erase_after(&this->_header, nullptr); }

    iterator
    before_begin() noexcept
    {
        return iterator(&this->_header);
    }

    const_iterator
    cbefore_begin() const noexcept
    {
        return const_iterator(&this->_header);
    }

    iterator
    begin() noexcept
    {
        return iterator(this->_header._next);
    }

    const_iterator
    cbegin() const noexcept
    {
        return const_iterator(this->_header._next);
    }

    iterator
    end() noexcept
    {
        return iterator(nullptr);
    }

    const_iterator
    cend() const noexcept
    {
        return const_iterator(nullptr);
    }

private:
    _Sgl_list_node_base _header;
    _Alloc _alloc;
    node_allocator _node_alloc;

    template <typename... _Args>
    node *
    _create_node(_Args &&..._args)
    {
        // Allocate memory for a new node. The memory has been uninitialized.
        node *_node = node_traits::allocate(_node_alloc, 1);

        try
        {
            // Initialize node to that memory region.
            ::new ((void *)_node) node;

            // Construct the value held by the node by forwarding a list of
            // parameters to construct an object.
            node_traits::construct(_node_alloc, _node->valptr(),
                                   std::forward<_Args>(_args)...);
        }
        catch (...)
        {
            node_traits::deallocate(_node_alloc, _node, 1);
            throw;
        }

        return _node;
    }

    void
    _default_initialize(size_type _n)
    {
        node_base *_start = &this->_header;

        for (; _n; --_n)
        {
            _start->_next = this->_create_node();
            _start        = _start->_next;
        }
    }
    void
    _fill_initialize(size_type _n, const value_type &_x)
    {
        node_base *_start = &this->_header;

        for (; _n; --_n)
        {
            _start->_next = this->_create_node(_x);
            _start        = _start->_next;
        }
    }

    node_base *
    _erase_after(node_base *_start, node_base *_end)
    {
        node *_curr = static_cast<node *>(_start->_next);

        while (_curr != static_cast<node *>(_end))
        {
            node *_tmp = _curr;
            _curr      = static_cast<node *>(_curr->_next);
            node_traits::destroy(_node_alloc, _tmp->valptr());
            _tmp->~node();
            node_traits::deallocate(_node_alloc, _tmp, 1);
        }

        _start->_next = _end;
        return _end;
    }
};
} // namespace opendsa

#endif /* __OPENDSA_SGL_LIST_H */
