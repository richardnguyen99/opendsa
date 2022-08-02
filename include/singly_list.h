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
#include <initializer_list>
#include <iterator>
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
    using _self     = _Sgl_list_const_iterator<_Tp>;
    using _iterator = _Sgl_list_iterator<_Tp>;

    using value_type      = _Tp;
    using reference       = const _Tp &;
    using pointer         = const _Tp *;
    using difference_type = std::ptrdiff_t;

    // Singly-linked-list is forward directional, which means it can only go
    // forward but cannot go backward. Therefore, only operator++() is
    // overloaded to make the iterator move forward and no operator--() is
    // overloaded to make the iterator move backward.
    using iterator_category = std::forward_iterator_tag;

    const _Sgl_list_node_base *_node;

    _Sgl_list_const_iterator() noexcept : _node() { }

    explicit _Sgl_list_const_iterator(const _Sgl_list_node_base *_n) noexcept
    : _node(_n)
    {
    }

    _Sgl_list_const_iterator(const _iterator &_iter) noexcept
    : _node(_iter._node)
    {
    }

    reference
    operator*() const noexcept
    {
        return *static_cast<const _Sgl_list_node<_Tp> *>(this->_node)->valptr();
    }

    pointer
    operator->() const noexcept
    {
        return static_cast<const _Sgl_list_node<_Tp> *>(this->_node)->valptr();
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

    /**
     * @brief Constructs an empty singly-linked list.
     */
    singly_list() : _header() { }

    /**
     * @brief Constructs a singly-linked list with @a _n of
     * default-inserted instances of type @a _Tp.
     *
     * @param _n Number of elements to insert.
     *
     * There is no copy happening but default constructor must be provided in
     * type @a _Tp if it's a user-defined type.
     */
    explicit singly_list(size_type _n) : _header()
    {
        this->_default_initialize(_n);
    }

    /**
     * @brief Constructs a singly-linked list with @a _n copies of elements with
     * value @a _x.
     *
     * @param _n Number of elements to insert.
     * @param _x Value to create copies.
     *
     * This constructor will initialize and fill the singly-linked list with _n
     * copies of element _x. If type @a _Tp is user-defined, a copy constructor
     * must be provided.
     */
    singly_list(size_type _n, const value_type &_x)
    {
        this->_fill_initialize(_n, _x);
    }

    /**
     * @brief Constructs a singly-linked list with data in range [_first,
     * _last).
     *
     * @param _first Iterator pointing to the first element in range.
     * @param _last  Iterator pointing to the last element in range exclusively.
     *
     * This constructor will initialize and fill the singly-linked list with the
     * data in range [_first, _last). If type @a _Tp is user-defined, a copy
     * constructor must be provided.
     */
    template <typename _InputIter,
              typename = typename std::enable_if<std::is_convertible<
                  typename std::iterator_traits<_InputIter>::iterator_category,
                  std::input_iterator_tag>::value>::type>
    singly_list(_InputIter _first, _InputIter _last)
    {
        this->_range_initialize(_first, _last);
    }

    /**
     * @brief Constructs a singly-linked list by copying data from _other.
     *
     * @param _other Another singly-linked list to copy.
     *
     * This constructor will initialize and fill the singly-linked list with
     * data in _other singly_list. If type @a _Tp is user-defined, a copy
     * constructor must be provided.
     */
    singly_list(const singly_list &_other)
    {
        this->_range_initialize(_other.cbegin(), _other.cend());
    }

    /**
     * @brief Constructs a singly-linked list by moving data from _other.
     *
     * @param _other Another singly-linked list to move.
     *
     * This constructor will initialize and fill the singly-linked list by
     * moving the data from _other. No copies are made.
     */
    singly_list(singly_list &&_other)
    {
        this->_header._next  = _other._header._next;
        _other._header._next = nullptr;
    }

    /**
     * @brief Constructs a singly-linked list with data from an initializer
     * list.
     *
     * @param _list An initializer list of type _Tp
     *
     * This constructor will initialize and fill the singly-linked list with
     * data in the initializer list (like C-style arrays). If @a _Tp is
     * user-defined, a copy constructor must be provided.
     */
    singly_list(std::initializer_list<_Tp> _list)
    {
        this->_range_initialize(_list.begin(), _list.end());
    }

    /**
     * @brief Destroys data and free allocated in the %singly_list
     */
    ~singly_list() { this->_erase_after(&this->_header, nullptr); }

    /**
     * @brief Returns a read/write reference to the first element of the
     * %singly_list.
     */
    reference
    front()
    {
        return *begin();
    }

    /**
     * @brief Returns a readonly reference to the first element of the
     * %singly_list.
     */
    const_reference
    front() const
    {
        return *cbegin();
    }

    /**
     * @brief Returns a read/write iterator that points to one before the first
     * element in the %singly_list.
     *
     * This iterator serves as a placeholder for modifications. Attempting to
     * access (dereference) it will result in undefined behavior.
     */
    iterator
    before_begin() noexcept
    {
        return iterator(&this->_header);
    }

    /**
     * @brief Returns a write iterator that points to one before the first
     * element in the %singly_list.
     *
     * This iterator serves as a placeholder for modifications. Attempting to
     * access (dereference) it will result in undefined behavior.
     */
    const_iterator
    cbefore_begin() const noexcept
    {
        return const_iterator(&this->_header);
    }

    /**
     * @brief Returns read/write iterator that points to the first element in
     * the %singly_list.
     */
    iterator
    begin() noexcept
    {
        return iterator(this->_header._next);
    }

    /**
     * @brief Returns readonly iterator that points to the first element in the
     * %singly_list.
     */
    const_iterator
    cbegin() const noexcept
    {
        return const_iterator(this->_header._next);
    }

    /**
     * @brief Returns a read/write iterator that points to one past the last
     * element in the %singly_list.
     */
    iterator
    end() noexcept
    {
        return iterator(nullptr);
    }

    /**
     * @brief Returns a readonly iterator that points to one past the last
     * element in the %singly_list.
     */
    const_iterator
    cend() const noexcept
    {
        return const_iterator(nullptr);
    }

    /**
     * @brief Returns true if the %singly_list is empty. (begin() == end())
     */
    [[nodiscard]] bool
    empty() const noexcept
    {
        return this->_header._next == nullptr;
    }

    /**
     * @brief Returns the largest possible number of elements of %singly_list.
     */
    size_type
    max_size() const noexcept
    {
        return node_traits::max_size(this->_node_alloc);
    }

    /**
     * @brief Erases all elements in the %singly_list.
     *
     * This method only removes the elements held in the %singly_list. If the
     * elements are pointers, the pointed-to-memory is not touched in any way.
     *
     * Any references, iterators or pointers, except those past-the-end,
     * referring to the container elements is invalidated.
     */
    void
    clear() noexcept
    {
        this->_erase_after(&this->_header, nullptr);
    }

    /**
     * @brief Inserts a new element after the specified position _pos in the
     * %singly_list.
     *
     * @param pos Iterator to insert a new data after.
     * @param value New data to insert.
     */
    iterator
    insert_after(const_iterator pos, const value_type &value)
    {
        return iterator(this->_insert_after(pos, value));
    }

    /**
     * @brief Inserts a new rvalue element after the specified position pos in
     * the %singly_list.
     *
     * @param pos Iterator to insert a new data after.
     * @param value New rvalue data to insert.
     */
    iterator
    insert_after(const_iterator pos, value_type &&value)
    {
        return iterator(this->_insert_after(pos, std::move(value)));
    }

    /**
     * @brief Inserts count copies of value value after the specified position
     * pos in the %singly_list.
     *
     * @param pos Iterator to insert a new data after.
     * @param count Number of elements to insert.
     * @param value Value to create copies.
     */
    iterator
    insert_after(const_iterator pos, size_type count, const value_type &value)
    {
        // Create a temporary singly-linked list
        singly_list _tmp(count, value);

        if (!_tmp.empty())
            // If that list is not empty, splice (move) the entire data to this
            // %singly_list.
            return this->_splice_after(pos, _tmp.cbefore_begin(), _tmp.cend());

        return iterator(const_cast<node_base *>(pos._node));
    }

    /**
     * @brief Inserts elements from range [first, last) after the specified
     * position pos in the %singly_list.
     *
     * @param pos Iterator to insert elements after.
     * @param first Iterator to indicate the lower bound inclusively.
     * @param last Iterator to indicate the upper bound exclusively.
     */
    template <typename _InputIter,
              typename = typename std::enable_if<std::is_convertible<
                  typename std::iterator_traits<_InputIter>::iterator_category,
                  std::input_iterator_tag>::value>::type>
    iterator
    insert_after(const_iterator pos, _InputIter first, _InputIter last)
    {
        // Create a temporary singly-linked list
        singly_list _tmp(first, last);

        if (!_tmp.empty())
            // If that list is not empty, splice (move) the entire data to this
            // %singly_list.
            return this->_splice_after(pos, _tmp.cbefore_begin(), _tmp.cend());

        return iterator(const_cast<node_base *>(pos._node));
    }

    /**
     * @brief Inserts elements from a given initializer list after the specified
     * position pos in the %singly_list.
     *
     * @param pos Iterator to insert elements after.
     * @param list Initializer list to insert values from.
     */
    iterator
    insert_after(const_iterator pos, std::initializer_list<value_type> list)
    {
        // Create a temporary singly-linked list
        singly_list _tmp(list.begin(), list.end());

        if (!_tmp.empty())
            // If that list is not empty, splice (move) the entire data to this
            // %singly_list.
            return this->_splice_after(pos, _tmp.cbefore_begin(), _tmp.cend());

        return iterator(const_cast<node_base *>(pos._node));
    }

    /**
     * @brief Inserts a new in-place-constructed element after the specified
     * position pos in the %singly_list.
     *
     * @param pos Iterator to insert a new element after.
     * @param args List of arguments to foward to the constructor of _Tp.
     *
     * This method will foward list of arguments and construct in-place a new
     * element. Thus, no copies or moves are made. The list of arguments has to
     * match with at least one constructor provided by _Tp.
     */
    template <typename... _Args>
    iterator
    emplace_after(const_iterator pos, _Args &&...args)
    {
        return iterator(this->_insert_after(pos, std::forward<_Args>(args)...));
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

    /**
     * @brief Helper method to initialize data from a given range. The range is
     * specified by iterators provided in the container, i.e. begin(), end().
     */
    template <typename _InputIterator>
    void
    _range_initialize(_InputIterator _first, _InputIterator _last)
    {
        node_base *_start = &this->_header;

        for (; _first != _last; ++_first)
        {
            _start->_next = this->_create_node(*_first);
            _start        = _start->_next;
        }
    }

    template <typename... _Args>
    node_base *
    _insert_after(const_iterator _pos, _Args &&..._args)
    {
        // Create a new node with object.
        node *_new_node  = this->_create_node(std::forward<_Args>(_args)...);
        node_base *_curr = const_cast<node_base *>(_pos._node);

        // Insert
        _new_node->_next = _curr->_next;
        _curr->_next     = _new_node;

        return _curr->_next;
    }

    /**
     * @brief Helper method that takes a range from another singly_list and
     * inserts (moves) to this %singly_list.
     *
     * @param _pos Iterator to insert the data after.
     * @param _before Iterator to indicate the range after.
     * @param _last Iterator to indaicate the range before.
     *
     * The range of data will be (_before, _last). To move the entire list, the
     * range should be (before_begin(), nullptr).
     */
    iterator
    _splice_after(const_iterator _pos, const_iterator _before,
                  const_iterator _last)
    {
        node_base *_curr = const_cast<node_base *>(_pos._node);
        node_base *_bptr = const_cast<node_base *>(_before._node);
        node_base *_eptr = _bptr;

        // Move elements in range (before, last) so the end pointer _eptr is the
        // one before _last.
        while (_eptr && _eptr->_next != _last._node)
            _eptr = _eptr->_next;

        if (_bptr != _eptr)
        {
            // Preserve the content inside the range.
            node_base *_preserve = _bptr->_next;

            if (_eptr)
            {
                // Update the new range in the other %singly_list.
                _bptr->_next = _eptr->_next;

                // Plug the new range into the right end.
                _eptr->_next = _curr->_next;
            }
            else
                _bptr->_next = nullptr;

            // Plug the new range into the left end.
            _curr->_next = _preserve;

            return iterator(_eptr);
        }

        return iterator(_curr);
    }

    /**
     * @brief Helper method to erase data in range (_start, _end)
     *
     * @param _start First pointer in range
     * @param _end Last pointer in range
     */
    node_base *
    _erase_after(node_base *_start, node_base *_end)
    {

        // Start to erase after _start
        node *_curr = static_cast<node *>(_start->_next);

        // If there are elements in range (_start, _end)
        while (_curr != static_cast<node *>(_end))
        {
            // Erase the current node to traverse to the next node iteratively
            node *_tmp = _curr;
            _curr      = static_cast<node *>(_curr->_next);

            node_traits::destroy(_node_alloc, _tmp->valptr());

            // Since nodes are directly constructed through placement new,
            // placement delete should be called as well.
            _tmp->~node();
            node_traits::deallocate(_node_alloc, _tmp, 1);
        }

        // Update the new singly-linked list
        _start->_next = _end;
        return _end;
    }
};
} // namespace opendsa

#endif /* __OPENDSA_SGL_LIST_H */
