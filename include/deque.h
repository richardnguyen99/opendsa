/**
 * @file deque.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief A doubly-ended queue
 * @version 0.2
 * @date 2022-07-02
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_DEQUE_H
#define __OPENDSA_DEQUE_H 1

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>

#include "helper.h"

#ifdef DEBUG
#include <iostream>
#include <typeinfo>
#endif

namespace opendsa
{

/**
 * @brief Compute the size of element for each node
 *
 * @param size_of_type  The size of element
 *
 * This function guarantees that each node in the map will hold the same
 * amount storage, or the same number of elements. This allows memory
 * management to be more uniform and consistenmemory management to be more
 * uniform and consistent.
 */
constexpr inline std::size_t
get_deque_buffer_size(std::size_t size_of_type)
{
    std::size_t DEQUE_DEFAULT_BUFFER_SIZE = 512;
    return (size_of_type < DEQUE_DEFAULT_BUFFER_SIZE)
               ? std::size_t(DEQUE_DEFAULT_BUFFER_SIZE / size_of_type)
               : std::size_t(1);
}

/**
 * @brief Deque iterator to hold information for a deque object
 *
 * A deque allocates a list of pointers. Each of them points to a fixed-size
 * array, which is determined by get_buffer_size_size(). For simplicity, a
 * list of pointers is called a map and each pointer is called a node. A
 * deque iterator will hold essential information about a particular node.
 * Each deque object will hold two of these to mark the entire map and its
 * associative nodes such as the range of the map, how many elements the
 * deque object is holding and how those elements are distributed.
 */
template <typename _Tp, typename _Ref, typename _Ptr>
struct deque_iterator
{
private:
    template <typename _CvTp>
    using _iter_template = deque_iterator<
        _Tp, _CvTp &,
        typename std::pointer_traits<_Ptr>::template rebind<_CvTp>>;

public:
    using iterator       = _iter_template<_Tp>;
    using const_iterator = _iter_template<const _Tp>;
    using node_pointer   = typename std::pointer_traits<_Ptr>::rebind<_Tp>;
    using map_pointer =
        typename std::pointer_traits<_Ptr>::rebind<node_pointer>;

    using iterator_category = std::random_access_iterator_tag;
    using value_type        = _Tp;
    using pointer           = _Ptr;
    using reference         = _Ref;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;

    // _curr behaves like the range of elements stored in the array in those
    // two essential iterators in a deque object. Additionally, it also
    // marks the current pointer to a particular element to support
    // traversal and other pointer-like computations such dereferencing.
    // This data member helps marking the range of elements in a deque
    // object.
    node_pointer _curr;
    node_pointer _first; // Mark the lower bound
    node_pointer _last;  // Mark the upper bound
    map_pointer _node;   // Mark the pointer in the map

    /**
     * @brief Returns the maximum number of objects of type _Tp this deque
     * iterator can hold.
     */
    static size_type
    get_nnodes() noexcept
    {
        return get_deque_buffer_size(sizeof(value_type));
    }

    /**
     * @brief Constructs an empty deque iterator.
     *
     */
    deque_iterator() noexcept : _curr(), _first(), _last(), _node() { }

    /**
     * @brief Constructs a new deque iterator object with existing data.
     *
     * @param nptr Node pointer to mark how many elements to store.
     * @param mptr Map pointer associating with the map.
     */
    deque_iterator(node_pointer nptr, map_pointer mptr) noexcept
    : _curr(nptr), _first(*mptr), _last(*mptr + get_nnodes()), _node(mptr)
    {
    }

    /**
     * @brief Converts normal iterator to const iterator.
     *
     * @param __x Normal deque iterator.
     */
    template <typename _Iter,
              typename = typename std::enable_if<
                  std::conjunction<std::is_same<deque_iterator, const_iterator>,
                                   std::is_same<_Iter, iterator>>::value>::type>
    deque_iterator(const _Iter &__x) noexcept
    : _curr(__x._curr), _first(__x._first), _last(__x._last), _node(__x._node)
    {
    }

    /**
     * @brief Copies an existing deque iterator.
     *
     * @param other Other deque iterator of the same.
     */
    deque_iterator(const deque_iterator &other) noexcept
    : _curr(other._curr), _first(other._first), _last(other._last),
      _node(other._node)
    {
    }

    deque_iterator &
    operator=(const deque_iterator &) = default;

    /**
     * @brief Dereferences the currently held data.
     */
    reference
    operator*() const noexcept
    {
        return *_curr;
    }

    /**
     * @brief Accesses data members and methods supported by type _Tp
     */
    pointer
    operator->() const noexcept
    {
        return _curr;
    }

    /**
     * @brief Prefix increments (increments then assigns)
     */
    deque_iterator &
    operator++() noexcept
    {
        ++_curr;
        if (_curr == _last)
        {
            set_node(_node + 1);
            _curr = _first;
        }

        return *this;
    }

    /**
     * @brief Postfix increments (assigns then increments)
     */
    deque_iterator
    operator++(int) noexcept
    {
        deque_iterator temp = *this;
        ++*this;
        return temp;
    }

    /**
     * @brief Prefix decrements (decrements then assigns)
     */
    deque_iterator &
    operator--() noexcept
    {
        if (_curr == _first)
        {
            set_node(_node - 1);
            _curr = _last;
        }
        --_curr;
        return *this;
    }

    /**
     * @brief Postfix decrements (assigns then decrements)
     */
    deque_iterator
    operator--(int) const noexcept
    {
        deque_iterator temp = *this;
        ++*this;
        return temp;
    }

    deque_iterator &
    operator+=(difference_type n) noexcept
    {
        // This overloaded operator accepts both positive and negative
        // values to traverse the %deque_iterator. Therefore, every
        // check should include both signs.
        const difference_type elm_offset = n + (_curr - _first);

        // If moving _curr does not exceed the current node
        if (elm_offset >= 0 && elm_offset < difference_type(get_nnodes()))
            _curr += n;
        else
        {
            // Compute node_offset based on the sign so that set_node() is
            // more capable of moving to the new node.
            const difference_type node_offset =
                elm_offset > 0
                    ? elm_offset / difference_type(get_nnodes())
                    : -difference_type((-elm_offset - 1) / get_nnodes()) -
                          1; // Rare case: _curr == _first

            set_node(_node + node_offset);
            _curr = _first // _first now is changed due to set_node
                    +
                    (elm_offset - node_offset * difference_type(get_nnodes()));
        }

        return *this;
    }

    deque_iterator &
    operator-=(difference_type n) noexcept
    {
        return (*this += -n);
    }

    reference
    operator[](difference_type n) const noexcept
    {
        return *(*this + n);
    }

    /**
     * @brief Mainly control traversal amongst nodes in the map
     *
     * @param node An existing node in the map
     *
     * This function will control what node in the map is held by this
     * %iterator. The information is the address of the node itself, and
     * the capacity. However, how many internal elements, i.e. the bound
     * aka @a _curr, is defined by the caller method.
     */
    void
    set_node(map_pointer node) noexcept
    {
        _node  = node;
        _first = *node;
        _last  = _first + get_nnodes();
    }

    // Comparisons between deques of two different template arguments are
    // for normal deques and const deques. If those operand deques have the
    // same template arguments, they're the same, either normal or const
    // deques.

    friend bool
    operator==(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return lhs._curr == rhs._curr;
    }

    template <typename _RefR, typename _PtrR>
    friend bool
    operator==(const deque_iterator &lhs,
               const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return lhs._curr == rhs._curr;
    }

    friend bool
    operator!=(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename _RefR, typename _PtrR>
    friend bool
    operator!=(const deque_iterator &lhs,
               const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend bool
    operator<(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return (lhs._node == rhs._node) ? (lhs._curr < rhs._curr)
                                        : (lhs._node < rhs._node);
    }

    template <typename _RefR, typename _PtrR>
    friend bool
    operator<(const deque_iterator &lhs,
              const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return (lhs._node == rhs._node) ? (lhs._curr < rhs._curr)
                                        : (lhs._node < rhs._node);
    }

    friend bool
    operator>(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return rhs < lhs;
    }

    template <typename _RefR, typename _PtrR>
    friend bool
    operator>(const deque_iterator &lhs,
              const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return rhs < lhs;
    }

    friend bool
    operator<=(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return !(rhs < lhs);
    }

    template <typename _RefR, typename _PtrR>
    friend bool
    operator<=(const deque_iterator &lhs,
               const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return !(rhs < lhs);
    }

    friend bool
    operator>=(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return !(lhs > rhs);
    }

    template <typename _RefR, typename _PtrR>
    friend bool
    operator>=(const deque_iterator &lhs,
               const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return !(lhs > rhs);
    }

    // operator-() between two deque iterators is to count how many elements
    // are stored in the two given deque iterators. Useful for some
    // functions such as std::distance().

    friend difference_type
    operator-(const deque_iterator &lhs, const deque_iterator &rhs) noexcept
    {
        return difference_type(get_nnodes()) *
                   (lhs._node - rhs._node - int(lhs._node != 0)) +
               (lhs._curr - lhs._first) + (rhs._last - rhs._curr);
    }

    template <typename _RefR, typename _PtrR>
    friend difference_type
    operator-(const deque_iterator &lhs,
              const deque_iterator<_Tp, _RefR, _PtrR> &rhs) noexcept
    {
        return difference_type(get_nnodes()) *
                   (lhs._node - rhs._node - int(lhs._node != 0)) +
               (lhs._curr - lhs._first) + (rhs._last - rhs._curr);
    }

    friend deque_iterator
    operator+(const deque_iterator &self, difference_type n) noexcept
    {
        deque_iterator tmp = self;
        tmp += n;
        return tmp;
    }

    friend deque_iterator
    operator+(difference_type n, const deque_iterator &self) noexcept
    {
        return (self + n);
    }

    friend deque_iterator
    operator-(const deque_iterator &self, difference_type n) noexcept
    {
        deque_iterator tmp = self;
        tmp -= n;
        return tmp;
    }

    friend deque_iterator
    operator-(difference_type n, const deque_iterator &self) noexcept
    {
        return (self - n);
    }
};

/**
 * @brief A doubly-ended queue
 *
 * @tparam _Tp Type of elements
 * @tparam _Alloc User-defined allocator
 *
 * A deque (deck) is a data structure that helps pushing elements at both
 * ends efficiently. A deque holds a list of pointers, called a map. Each
 * pointer holds the address of, or points to, a fixed-size array that
 * stores actual data. The size of the array is determined depending on the
 * size of the type of its elements.
 */
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class deque
{
private:
    // Mainly in charge of allocating individual elements in the deque.

    using _Tp_alloc_type =
        typename std::allocator_traits<_Alloc>::rebind_alloc<_Tp>;
    using _Tp_alloc_traits = std::allocator_traits<_Tp_alloc_type>;
    using _Tp_ptr          = typename _Tp_alloc_traits::pointer;
    using _Tp_ptr_const    = typename _Tp_alloc_traits::const_pointer;

    // If users have specified their own allocator, reuse for allocating
    // pointers in the map.

    using _Map_alloc_type =
        typename std::allocator_traits<_Alloc>::rebind_alloc<_Tp_ptr>;
    using _Map_alloc_traits = std::allocator_traits<_Map_alloc_type>;

public:
    // Type aliases

    using value_type       = _Tp;
    using reference        = _Tp &;
    using const_reference  = const _Tp &;
    using pointer          = _Tp_ptr;
    using const_pointer    = _Tp_ptr_const;
    using size_type        = std::size_t;
    using difference_type  = std::ptrdiff_t;
    using iterator         = deque_iterator<_Tp, _Tp &, _Tp_ptr>;
    using const_iterator   = deque_iterator<_Tp, const _Tp &, _Tp_ptr_const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using node_pointer = typename iterator::node_pointer;
    using map_pointer  = typename iterator::map_pointer;

    /**
     * @brief Creates an empty %deque.
     */
    deque() : _start(), _finish(), _map(), _map_size() { _initialize_map(0); }

    /**
     * @brief Creates a %deque filled with default constructed elements.
     *
     * @param count The number of elements.
     *
     * This constructor creates a deque object by filling it with `n`
     * number of default values of `_Tp`.
     */
    explicit deque(size_type count) : _start(), _finish(), _map(), _map_size()
    {
        _initialize_map(count);
        _fill_construct(value_type());
    }

    /**
     * @brief Creates a %deque with copies of a given element.
     *
     * @param count The number of elements.
     * @param value An element to copy.
     *
     * This constructor creates a deque object by filling it with @a n
     * copies of @a value.
     */
    explicit deque(size_type count, const value_type &value)
    : _start(), _finish(), _map(), _map_size()
    {
        _initialize_map(count);
        _fill_construct(value);
    }

    /**
     * @brief Creates a %deque based on a range of elements.
     *
     * @param first An input iterator to mark the range.
     * @param last  An input iterator to mark the range.
     *
     * This constructor creates a deque object by copying the elements
     * from [first, last).
     */
    template <typename _InputIter,
              typename = typename std::enable_if<std::is_convertible<
                  typename std::iterator_traits<_InputIter>::iterator_category,
                  std::input_iterator_tag>::value>::type>
    deque(_InputIter first, _InputIter last)
    {
        typename std::iterator_traits<_InputIter>::iterator_category
            iter_traits =
                typename std::iterator_traits<_InputIter>::iterator_category();

        _range_construct(first, last, iter_traits);
    }

    /**
     * @brief Creates a %deque based on an initializer list.
     *
     * @param list  An initializer list.
     *
     * This constructor creates a deque object by copying the elements
     * in the initializer list.
     */
    deque(std::initializer_list<value_type> list)
    {
        _range_construct(list.begin(), list.end(),
                         std::random_access_iterator_tag());
    }

    /**
     * @brief Creates a %deque using deep copying.
     *
     * @param other An existing deque object.
     *
     * This constructor creates a deque object by copying the elements in
     * the other deque object without dangling or compromise between each
     * according element in both objects.
     */
    deque(const deque &other)
    {
        _range_construct(other.cbegin(), other.cend(),
                         std::random_access_iterator_tag());
    }

    deque(deque &&other)
    {
        this->_start    = other._start;
        this->_finish   = other._finish;
        this->_map      = other._map;
        this->_map_size = other._map_size;

        other._start    = iterator();
        other._finish   = iterator();
        other._map      = nullptr;
        other._map_size = 0;
    }

    /**
     * @brief Destructor erases elements and reclaims memory.
     *
     *  The dtor only erases the elements, and note that if the elements
     *  themselves are pointers, the pointed-to memory is not touched in
     * any way.  Managing the pointer is the user's responsibility.
     */
    ~deque()
    {
        _destroy_data(this->_start, this->_finish);
        if (this->_map)
        {
            _deallocate_nodes(this->_start._node, this->_finish._node + 1);
            _deallocate_map(this->_map, this->_map_size);
        }
    }

    deque &
    operator=(const deque &x)
    {
        if (&x != this)
        {
            const size_type len = size();
            if (len > x.size())
            {
                iterator delete_start =
                    std::copy(x.cbegin(), x.cend(), this->_start);
                _erase_to_end(delete_start);
            }
            else
            {
                const_iterator mid = x.cbegin() + difference_type(len);
                std::copy(x.cbegin(), mid, this->_start);
                _range_insert_aux(this->_finish, mid, x.cend(),
                                  std::random_access_iterator_tag());
            }
        }

        return *this;
    }

    deque &
    operator=(deque &&x) noexcept
    {
        this->swap(x);
        x.clear();

        return *this;
    }

    deque &
    operator=(std::initializer_list<value_type> l)
    {
        const size_type len = l.size();
        if (len > size())
        {
            typename std::initializer_list<value_type>::iterator mid =
                l.begin();
            std::advance(mid, size());
            std::copy(l.begin(), mid, begin());
            _range_insert_aux(end(), mid, l.end(),
                              std::random_access_iterator_tag());
        }
        else
        {
            iterator delete_start = std::copy(l.begin(), l.end(), begin());
            _erase_to_end(delete_start);
        }

        return *this;
    }

    // Element access methods

    reference
    at(size_type pos)
    {
        if (pos >= this->size())
        {
            const std::string msg = "Index " + std::string(pos) +
                                    " is out of bound, which is " +
                                    std::string(this->size());
            throw std::runtime_error(msg);
        }

        return (*this)[pos];
    }

    const_reference
    at(size_type pos) const
    {
        if (pos >= this->size())
        {
            const std::string msg = "Index " + std::string(pos) +
                                    " is out of bound, which is " +
                                    std::string(this->size());
            throw std::runtime_error(msg);
        }

        return (*this)[pos];
    }

    reference
    operator[](size_type pos)
    {
        return this->_start[difference_type(pos)];
    }

    const_reference
    operator[](size_type pos) const
    {
        return this->_start[difference_type(pos)];
    }

    /**
     * @brief Returns a reference to the first element in the container.
     *
     * This function will return a reference to the first element in the
     * container, similar to `*c.begin()` where c is a deque object. Calling
     * `front()` on an empty deque object is undefined behavior.
     */
    reference
    front() noexcept
    {
        _NON_EMPTY_DEQUE("Can't use front() on an empty deque");
        return *begin();
    }

    /**
     * @brief Returns a const reference to the first element in the
     * container.
     *
     * This function will return a reference to the first element in the
     * container, similar to `*c.cbegin()` where c is a deque object.
     * Calling `front()` on an empty deque object is undefined behavior.
     */
    const_reference
    front() const noexcept
    {
        return *cbegin();
    }

    /**
     * @brief Returns a reference to the last element in the container.
     *
     * This function will return a reference to the last element in
     * the container, similar to `*(c.end() - 1)` where is c is a deque
     * object. Calling `back()` on an empty deque object is undefined
     * behavior.
     */
    reference
    back() noexcept
    {
        iterator tmp = end();
        --tmp;
        return *tmp;
    }

    /**
     * @brief Returns a const reference to the last element in the
     * container.
     *
     * This function will return a const reference to the last element in
     * the container, similar to `*(c.end() - 1)` where is c is a deque
     * object. Calling `back()` on an empty deque object is undefined
     * behavior.
     */
    const_reference
    back() const noexcept
    {
        const_iterator tmp = cend();
        --tmp;
        return *tmp;
    }

    /**
     * @brief Returns a read/write iterator pointing to the first element in
     * the %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `begin()` is same as `end()`.
     */
    iterator
    begin() noexcept
    {
        return this->_start;
    }

    /**
     * @brief Returns a read-only iterator pointing to the first element in
     * the %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `begin()` is same as `end()`.
     */
    const_iterator
    cbegin() const noexcept
    {
        return this->_start;
    }

    /**
     * @brief Returns a read/write iterator pointing to the element after
     * the last element in the %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `begin()` is same as `end()`.
     *
     * `end()` and `cend()` are considered as placeholders to support other
     * operations such as comparison, traversal and modification. Attempts
     * to access results in undefined behavior.
     */
    iterator
    end() noexcept
    {
        return this->_finish;
    }

    /**
     * @brief Returns a read-only iterator pointing to the element after
     * the last element in the %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `cbegin()` is same as `cend()`.
     *
     * `end()` and `cend()` are considered as placeholders to support other
     * operations such as comparison, traversal and modification. Attempts
     * to access results in undefined behavior.
     */
    const_iterator
    cend() const noexcept
    {
        return this->_finish;
    }

    /**
     * @brief Returns a read/write iterator pointing the first element in
     * the reversed %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `rbegin()` is same as `rend()`.
     *
     * It corresponds to the last element of the non-reversed %deque.
     */
    reverse_iterator
    rbegin() noexcept
    {
        return reverse_iterator(this->_finish);
    }

    /**
     * @brief Returns a read-only iterator pointing the first element in
     * the reversed %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `crbegin()` is same as `crend()`.
     *
     * It corresponds to the last element of the non-reversed %deque.
     */
    const_reverse_iterator
    crbegin() const noexcept
    {
        return const_reverse_iterator(this->_finish);
    }

    /**
     * @brief Returns a read/write iterator pointing the element after the
     * last element in the reversed %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `cbegin()` is same as `cend()`.
     *
     * `rend()` and `crend()` are considered as placeholders to support
     * other operations such as comparison, traversal and modification.
     * Attempts to access results in undefined behavior.
     *
     * It corresponds to the element before the first element of the
     * non-reversed %deque.
     */
    reverse_iterator
    rend() noexcept
    {
        return reverse_iterator(this->_start);
    }

    /**
     * @brief Returns a read-only iterator pointing the element after the
     * last element in the reversed %deque.
     *
     * Iterator in %deque is tagged as `std::random_access_iterator_tag`. If
     * %deque is empty, `crbegin()` is same as `crend()`.
     *
     * `rend()` and `crend()` are considered as placeholders to support
     * other operations such as comparison, traversal and modification.
     * Attempts to access results in undefined behavior.

     * It corresponds to the element before the first element of the
     * non-reversed %deque.
     */
    const_reverse_iterator
    crend() const noexcept
    {
        return const_reverse_iterator(this->_start);
    }

    // Capacities

    /**
     * @brief Returns the number of elements in the %deque
     */
    size_type
    size() const noexcept
    {
        return this->_finish - this->_start;
    }

    /**
     * @brief Returns whether or not the %deque is empt, i.e begin() ==
     * end()
     */
    bool
    empty() const noexcept
    {
        return this->_finish == this->_start;
    }

    /**
     * @brief Returns the maximum number of elements that the %deque could
     * hold
     */
    size_type
    max_size() const noexcept
    {
        return _Tp_alloc_traits::max_size(_alloc);
    }

    /**
     * @brief Inserts a new element into the container directly before the
     * first element.
     *
     * @param args Argument list to construct in-place a new element.
     *
     * `emplace` functions will construct the element in-place based on the
     * list of arguments ...args.
     */

    template <typename... Args>
    reference
    emplace_front(Args &&...args)
    {
        if (this->_start._curr != this->_start._first)
        {
            _Tp_alloc_traits::construct(_alloc, this->_start._curr - 1,
                                        std::forward<Args>(args)...);
            this->_start._curr--;
        }
        else
            _push_front_aux(std::forward<Args>(args)...);

        return front();
    }

    /**
     * @brief Inserts a new element into the container direcly at end().
     *
     * @param args Argument list to construct in-place a new element.
     *
     * `emplace` functions will construct the element in-place based on the
     * list of arguments ...args.
     */
    template <typename... Args>
    reference
    emplace_back(Args &&...args)
    {
        if (this->_finish._curr != this->_finish._last - 1)
        {
            _Tp_alloc_traits::construct(_alloc, this->_finish._curr,
                                        std::forward<Args>(args)...);
            ++this->_finish._curr;
        }
        else
            _push_back_aux(std::forward<Args>(args)...);

        return back();
    }

    /**
     * @brief Inserts a new element into the container directly before
     position.
     *
     * @param position Position to insert a new element before.
     * @param args Argument list to construct in-place a new element.
     *
     * `emplace` functions will construct the element in-place based on the
     * list of arguments ...args.
     */
    template <typename... Args>
    iterator
    emplace(const_iterator position, Args &&...args)
    {
        if (position._curr == this->_start._curr)
        {
            emplace_front(std::forward<Args>(args)...);
            return this->_start;
        }
        else if (position._curr == this->_finish._curr)
        {
            emplace_back(std::forward<Args>(args)...);
            return this->_finish;
        }

        return _insert_aux(begin() + (position - cbegin()),
                           std::forward<Args>(args)...);
    }

    /**
     * @brief Adds an element into the first of the %deque.
     *
     * @param x Element to be added.
     */
    void
    push_front(const value_type &x)
    {
        if (this->_start._curr != this->_start._first)
        {
            _Tp_alloc_traits::construct(_alloc, this->_start._curr - 1, x);
            --this->_start._curr;
        }
        else
            _push_front_aux(x);
    }

    /**
     * @brief Adds an rvalue element into the first of the %deque.
     *
     * @param x Rvalue element to be added.
     */
    void
    push_front(value_type &&x)
    {
        emplace_front(std::move(x));
    }

    /**
     * @brief Adds an element into the back of the %deque.
     *
     * @param x Element to be added.
     */
    void
    push_back(const value_type &x)
    {
        if (this->_finish._curr != this->_finish._last - 1)
        {
            _Tp_alloc_traits::construct(_alloc, this->_finish._curr, x);
            ++this->_finish._curr;
        }
        else
            _push_back_aux(x);
    }

    /**
     * @brief Adds an rvalue element into the back of the %deque.
     *
     * @param x Rvalue element to be added.
     */
    void
    push_back(value_type &&x)
    {
        emplace_back(std::move(x));
    }

    iterator
    insert(const_iterator position, const value_type &x)
    {
        if (position._curr == this->_start._curr)
        {
            push_front(x);
            return this->_start;
        }

        if (position._curr == this->_finish._curr)
        {
            push_back(x);
            iterator tmp = this->_start._finish;
            --tmp;
            return tmp;
        }

        return _insert_aux(begin() + (position - cbegin()), x);
    }

    /**
     * @brief Inserts given rvalue into %deque directly before specified
     * iterator.
     *
     * @param position  Iterator to insert the value before.
     * @param x The rvalue to be inserted.
     *
     * This function uses deep copy technique to insert a copy of the given
     * value before the specified position. Thus, if type _Tp is a
     * user-defined class, a copy constructor must be provided.
     */
    iterator
    insert(const_iterator position, value_type &&x)
    {
        return emplace(position, std::move(x));
    }

    /**
     * @brief Inserts @a count numbers of a specified value of type @a _Tp
     * into %deque directly before specified iterator.
     *
     * @param position Iterator to insert new elements.
     * @param count Number of elements.
     * @param x The element to create copies.
     *
     * This function uses deep copy technique to insert @a count copies of
     * the given value before the specified position. Thus, if type @a _Tp
     * is a user-defined class, a copy constructor must be provided.
     */
    iterator
    insert(const_iterator position, size_type count, const value_type &x)
    {
        difference_type offset = position - cbegin();
        _fill_insert_aux(begin() + offset, count, x);
        return begin() + offset;
    }

    /**
     * @brief Inserts a list of elements of same type into %deque directly
     * before specified iterator.
     *
     * @param position Iterator to insert the list before.
     * @param l Initializer list of elements
     *
     * This function copies elements from @a l to the location before the
     * specified iterator. Thus, if type @a _Tp is a user-defined lcase, a
     * copy constructor must be provided.
     */
    iterator
    insert(const_iterator position, std::initializer_list<value_type> l)
    {
        difference_type offset = position - cbegin();

        _range_insert_aux(begin() + offset, l.begin(), l.end(),
                          std::random_access_iterator_tag());

        return begin() + offset;
    }

    /**
     * @brief Removes the first element.
     *
     * pop_front() basically removes the first element on the deque. This
     * will shrink the size by one. It also returns no value so if the first
     * element is needed before being popped, use front() first.
     */
    void
    pop_front() noexcept
    {
        _NON_EMPTY_DEQUE("Cannot use pop_front() on an empty deque");
        if (this->_start._curr != this->_start._last - 1)
        {
            _Tp_alloc_traits::destroy(_alloc, this->_start._curr);
            ++this->_start._curr;
        }
        else
            _pop_front_aux();
    }

    /**
     * @brief Removes the last element.
     *
     * pop_back() basically removes the last element on the deque. This will
     * shrink the size by one. It also returns no value so if the last
     * element is needed before popped, use back() first.
     */
    void
    pop_back() noexcept
    {
        _NON_EMPTY_DEQUE("Cannot use pop_back() on an empty deque");
        if (this->_finish._curr != this->_finish._first)
        {
            --this->_finish._curr;
            _Tp_alloc_traits::destroy(_alloc, this->_finish._curr);
        }
        else
            _pop_back_aux();
    }

    /**
     * @brief Removes the element at position.
     *
     * @param position Iterator where to delete the element.
     *
     * erase() will erase the element at the given position, so it will
     * shirnk the size of the deque by one.
     */
    iterator
    erase(const_iterator position)
    {
        return _erase(begin() + (position - cbegin()));
    }

    /**
     * @brief Removes the elements in range [first, last).
     *
     * @param first Iterator to the first element to be deleted.
     * @param last Iterator to one past the last element to be deleted.
     */
    iterator
    erase(const_iterator first, const_iterator last)
    {
        return _erase_range(begin() + (first - cbegin()),
                            begin() + (last - cbegin()));
    }

    /**
     * @brief Resizes the deque to `count`.
     *
     * @param count New number of elements the deque should contain.
     *
     * resize() will modify the current size of the deque to the new size,
     * count. If count < size, the deque will be truncated. If count > size,
     * new default constructed elements of type _Tp are appended at the end.
     */
    void
    resize(size_type count)
    {
        resize(count, value_type());
    }

    /**
     * @brief Resizes the deque to `count`.
     *
     * @param count New number of elements the deque should contain.
     * @param value Data to be appended.
     *
     * resize() will modify the current size of the deque to the new size,
     * count. if count < size, the deque will be truncated. If count > size,
     * copies of `value` are appended at the end.
     */
    void
    resize(size_type count, const value_type &value)
    {
        const size_type len = size();
        if (count > len)
            _insert_aux(end(), count - len, value);
        else if (count < len)
            _erase_to_end(this->_start + difference_type(count));
    }

    /**
     * @brief Erases all the elements in the deque.
     *
     * clear() will erase all the elements currently in the deque. If the
     * elements are pointers, the memory they point to will remain, and
     * require manual managment. After this call, size() will return 0.
     */
    void
    clear()
    {
        _erase_to_end(begin());
    }

    /**
     * @brief Swaps the content between two deques.
     *
     * @param other Another deque of the same element and allocator type.
     *
     * swap() guarantees fast content swapping in constant time, which means
     * there is no copying, moving, allocating or deallocating whatsoever.
     */
    void
    swap(deque &other) noexcept
    {
        map_pointer tmp_map = other._map;
        size_type tmp_size  = other._map_size;
        iterator tmp_start  = other._start;
        iterator tmp_finish = other._finish;

        other._map      = this->_map;
        other._map_size = this->_map_size;
        other._start    = this->_start;
        other._finish   = this->_finish;

        this->_map      = tmp_map;
        this->_map_size = tmp_size;
        this->_start    = tmp_start;
        this->_finish   = tmp_finish;
    }

private:
    constexpr static size_type INITIAL_MAP_SIZE = 8;

    iterator _start;
    iterator _finish;
    map_pointer _map;
    size_type _map_size;
    _Tp_alloc_type _alloc;
    _Map_alloc_type _map_alloc;

    static size_type
    _max_nodes()
    {
        return get_deque_buffer_size(sizeof(value_type));
    }

    void
    _destroy_data(iterator first, iterator last)
    {
        for (map_pointer curr = first._node + 1; curr < last._node; curr++)
        {
            for (node_pointer node_curr = *curr;
                 node_curr < *curr + _max_nodes(); node_curr++)
                _Tp_alloc_traits::destroy(_alloc, std::addressof(*node_curr));
        }

        if (first._node != last._node)
        {
            for (node_pointer curr = first._curr; curr != first._last; curr++)
                _Tp_alloc_traits::destroy(_alloc, std::addressof(*curr));
            for (node_pointer curr = last._first; curr != last._curr; curr++)
                _Tp_alloc_traits::destroy(_alloc, std::addressof(*curr));
        }
        else
        {
            for (node_pointer curr = first._curr; curr != last._curr + 1;
                 curr++)
                _Tp_alloc_traits::destroy(_alloc, std::addressof(*curr));
        }
    }

    void
    _deallocate_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer curr = first; curr < last; curr++)
            _Tp_alloc_traits::deallocate(_alloc, std::addressof(**curr),
                                         _max_nodes());
    }

    void
    _deallocate_map(map_pointer map, size_type map_size)
    {
        _Map_alloc_traits::deallocate(_map_alloc, this->_map, this->_map_size);
    }

    void
    _create_nodes(map_pointer map_start, map_pointer map_finish)
    {

        map_pointer curr;
        try
        {
            for (curr = map_start; curr < map_finish; curr++)
            {
                *curr = _Tp_alloc_traits::allocate(_alloc, _max_nodes());
            }
        }
        catch (...)
        {
            _deallocate_nodes(map_start, curr);
            throw;
        }
    }

    /**
     * This helper function initializes a doubly-ended queue that
     * contains an array of pointers. Each pointer holds an address of a
     * fixed-size array of elements where the size is determined over
     * the size of each element.
     */
    void
    _initialize_map(size_type num_elms)
    {
        const size_type num_nodes = (num_elms / _max_nodes() + 1);

        this->_map_size = std::max(INITIAL_MAP_SIZE, size_type(num_nodes + 2));
        this->_map = _Map_alloc_traits::allocate(_map_alloc, this->_map_size);

        for (size_type i = 0; i < this->_map_size; i++)
            this->_map[i] = nullptr;

        map_pointer map_start  = this->_map + (this->_map_size - num_nodes) / 2;
        map_pointer map_finish = map_start + num_nodes;

        try
        {
            _create_nodes(map_start, map_finish);
        }
        catch (...)
        {
            _deallocate_map(this->_map, this->_map_size);
            this->_map      = map_pointer();
            this->_map_size = 0;
            throw;
        }

        this->_start.set_node(map_start);
        this->_finish.set_node(map_finish - 1);
        this->_start._curr  = this->_start._first;
        this->_finish._curr = this->_finish._first + num_elms % _max_nodes();
    }

    void
    _reallocate_map(size_type nodes_to_add, bool at_front)
    {
        const size_type old_num_nodes =
            this->_finish._node - this->_start._node + 1;
        const size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_map_start;
        if (this->_map_size > 2 * new_num_nodes)
        {
            new_map_start = this->_map + (this->_map_size - new_num_nodes) / 2 +
                            (at_front ? nodes_to_add : 0);

            if (new_map_start < this->_start._node)
                std::copy(this->_start._node, this->_finish._node + 1,
                          new_map_start);
            else
                std::copy_backward(this->_start._node, this->_finish._node + 1,
                                   new_map_start + old_num_nodes);
        }
        else
        {
            const size_type new_map_size =
                this->_map_size + std::max(this->_map_size, nodes_to_add) + 2;

            map_pointer new_map =
                _Map_alloc_traits::allocate(_map_alloc, new_map_size);
            new_map_start = new_map + (new_map_size - new_num_nodes) / 2 +
                            (at_front ? nodes_to_add : 0);
            std::copy(this->_start._node, this->_finish._node + 1,
                      new_map_start);
            this->_deallocate_map(this->_map, this->_map_size);
            this->_map      = new_map;
            this->_map_size = new_map_size;
        }

        this->_start.set_node(new_map_start);
        this->_finish.set_node(new_map_start + old_num_nodes - 1);
    }

    void
    _reserve_map_at_front(size_type nodes_to_add = 1)
    {
        if (nodes_to_add > size_type(this->_start._node - this->_map))
            _reallocate_map(nodes_to_add, true);
    }

    void
    _reserve_map_at_back(size_type nodes_to_add = 1)
    {
        if (nodes_to_add + 1 >
            this->_map_size - (this->_finish._node - this->_map))
            _reallocate_map(nodes_to_add, false);
    }

    void
    _new_elements_at_front(size_type new_elms)
    {
        if (this->max_size() - this->size() < new_elms)
            throw std::runtime_error("cannot create opendsa::deque larger "
                                     "than max_size(), which is " +
                                     this->max_size());

        const size_type new_nodes =
            ((new_elms + _max_nodes() - 1) / _max_nodes());
        _reserve_map_at_front(new_nodes);
        size_type i;

        try
        {
            for (i = 1; i <= new_nodes; ++i)
                *(this->_start._node - i) =
                    _Tp_alloc_traits::allocate(_alloc, _max_nodes());
        }
        catch (...)
        {
            for (size_type j = 1; j < i; j++)
                _Tp_alloc_traits::deallocate(_alloc, *(this->_finish._node - j),
                                             _max_nodes());
            throw;
        }
    }

    void
    _new_elements_at_back(size_type new_elms)
    {
        if (this->max_size() - this->size() < new_elms)
            throw std::runtime_error("cannot create opendsa::deque larger "
                                     "than max_size(), which is " +
                                     this->max_size());

        const size_type new_nodes =
            ((new_elms + _max_nodes() - 1) / _max_nodes());
        _reserve_map_at_back(new_nodes);
        size_type i;

        try
        {
            for (i = 1; i <= new_nodes; i++)
                *(this->_finish._node + i) =
                    _Tp_alloc_traits::allocate(_alloc, _max_nodes());
        }
        catch (...)
        {
            for (size_type j = 1; j < i; j++)
                _Tp_alloc_traits::deallocate(_alloc, *(this->_finish._node - j),
                                             _max_nodes());
            throw;
        }
    }

    iterator
    _reserve_elements_at_front(size_type n)
    {
        const size_type move_nodes = this->_start._curr - this->_start._first;

        if (n > move_nodes)
            _new_elements_at_front(n - move_nodes);

        return this->_start - difference_type(n);
    }

    iterator
    _reserve_elements_at_back(size_type n)
    {
        const size_type move_nodes =
            this->_finish._last - this->_finish._curr - 1;

        if (n > move_nodes)
            _new_elements_at_back(n - move_nodes);

        return this->_finish + difference_type(n);
    }

    void
    _fill_construct(const value_type &value)
    {
        map_pointer curr;
        try
        {
            for (curr = this->_start._node; curr < this->_finish._node; ++curr)
            {
                for (node_pointer node_curr = *curr;
                     node_curr < *curr + _max_nodes(); node_curr++)
                    _Tp_alloc_traits::construct(
                        _alloc, std::addressof(*node_curr), value);
            }

            for (node_pointer node_curr = this->_finish._first;
                 node_curr < this->_finish._curr; node_curr++)
                _Tp_alloc_traits::construct(_alloc, std::addressof(*node_curr),
                                            value);
        }
        catch (...)
        {
            for (map_pointer del_curr = this->_start._node; del_curr < curr;
                 del_curr++)
            {
                for (node_pointer node_curr = *del_curr;
                     node_curr < *del_curr + _max_nodes(); node_curr++)
                    _Tp_alloc_traits::destroy(_alloc,
                                              std::addressof(*node_curr));
            }
            throw;
        }
    }

    template <typename _InputIter>
    void
    _range_construct(_InputIter first, _InputIter last,
                     std::forward_iterator_tag)
    {
        const size_type n = std::distance(first, last);
        this->_initialize_map(n);

        map_pointer curr;
        try
        {
            for (curr = this->_start._node; curr < this->_finish._node; ++curr)
            {
                _InputIter mid = first;
                _InputIter tmp = first;
                std::advance(mid, _max_nodes());

                for (size_type i = 0; tmp < mid; tmp++, i++)
                {
                    _Tp_alloc_traits::construct(
                        _alloc, std::addressof(*(*curr + i)), *tmp);
                }
                first = mid;
            }

            _InputIter tmp = first;
            for (size_type i = 0; tmp < last; tmp++, i++)
            {
                _Tp_alloc_traits::construct(_alloc,
                                            std::addressof(*(*curr + i)), *tmp);
            }
        }
        catch (...)
        {
            for (map_pointer del_curr = this->_start._node; del_curr < curr;
                 del_curr++)
            {
                for (node_pointer node_curr = *del_curr;
                     node_curr < *del_curr + _max_nodes(); node_curr++)
                    _Tp_alloc_traits::destroy(_alloc,
                                              std::addressof(*node_curr));
            }
        }
    }

    /**
     * Helper function to insert when _start._curr == _start._first happens
     */
    template <typename... Args>
    void
    _push_front_aux(Args &&...args)
    {
        if (size() == max_size())
            throw std::runtime_error("cannot create opendsa::deque larger "
                                     "than max_size(), which is " +
                                     this->max_size());

        _reserve_map_at_front();
        *(this->_start._node - 1) =
            _Tp_alloc_traits::allocate(_alloc, _max_nodes());

        try
        {
            this->_start.set_node(this->_start._node - 1);
            this->_start._curr = this->_start._last - 1;
            _Tp_alloc_traits::construct(_alloc, this->_start._curr,
                                        std::forward<Args>(args)...);
        }
        catch (...)
        {
            ++this->_start;
            _Tp_alloc_traits::deallocate(_alloc, *(this->_start._node - 1),
                                         _max_nodes());
            throw;
        }
    }

    /**
     * Helper function to insert when _finish._curr == _finish._last - 1
     * happens
     */
    template <typename... Args>
    void
    _push_back_aux(Args &&...args)
    {
        if (size() == max_size())
            throw std::runtime_error("cannot create opendsa::deque larger "
                                     "than max_size(), which is " +
                                     this->max_size());

        _reserve_map_at_back();
        *(this->_finish._node + 1) =
            _Tp_alloc_traits::allocate(_alloc, _max_nodes());

        try
        {
            _Tp_alloc_traits::construct(_alloc, this->_finish._curr,
                                        std::forward<Args>(args)...);
            this->_finish.set_node(this->_finish._node + 1);
            this->_finish._curr = this->_finish._first;
        }
        catch (...)
        {
            _Tp_alloc_traits::deallocate(_alloc, *(this->_finish._node + 1),
                                         _max_nodes());
            throw;
        }
    }

    template <typename... Args>
    iterator
    _insert_aux(iterator position, Args &&...args)
    {
        value_type copy(std::forward<Args>(args)...);

        difference_type index = position - this->_start;
        if (static_cast<size_type>(index) < size() / 2)
        {
            push_front(std::move(front()));
            iterator front1 = iterator(this->_start + 1);
            iterator front2 = iterator(front1 + 1);
            position        = this->_start + index;
            iterator pos1   = iterator(position + 1);

            std::move(front2, pos1, front1);
        }
        else
        {
            push_back(std::move(back()));
            iterator back1 = iterator(this->_finish - 1);
            iterator back2 = iterator(back1 - 1);
            position       = this->_start + index;
            std::move_backward(position, back2, back1);
        }

        *position = std::move(copy);
        return position;
    }

    template <typename _ForwardIter>
    void
    _insert_aux(iterator pos, _ForwardIter first, _ForwardIter last,
                size_type n)
    {
        const difference_type elms_before = pos - this->_start;
        const size_type length            = size();

        if (static_cast<size_type>(elms_before) < length / 2)
        {
            iterator new_start = _reserve_elements_at_front(n);
            iterator old_start = this->_start;
            pos                = this->_start + elms_before;

            try
            {
                if (elms_before >= difference_type(n))
                {
                    iterator nstart = (this->_start + difference_type(n));
                    __uninit_move_with_allocator(this->_start, nstart,
                                                 new_start, _alloc);
                    this->_start = new_start;
                    std::move(nstart, pos, old_start);
                    std::copy(first, last, pos - difference_type(n));
                }
                else
                {
                    _ForwardIter mid = first;
                    std::advance(mid, difference_type(n) - elms_before);
                    const difference_type start_to_pos = (pos - this->_start);
                    __uninit_move_with_allocator(this->_start, pos, new_start,
                                                 _alloc);
                    __uninit_copy_with_allocator(
                        first, mid, new_start + start_to_pos, _alloc);
                    this->_start = new_start;
                    std::copy(mid, last, old_start);
                }
            }
            catch (...)
            {
                for (map_pointer mcurr = new_start._node;
                     mcurr < this->_start._node; mcurr++)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
        else
        {
            iterator new_finish = _reserve_elements_at_back(n);
            iterator old_finish = this->_finish;
            const difference_type elms_after =
                difference_type(length) - elms_before;
            pos = this->_finish - elms_after;
            try
            {
                if (elms_after > difference_type(n))
                {
                    iterator nfinish = (this->_finish - difference_type(n));
                    __uninit_move_with_allocator(nfinish, this->_finish,
                                                 this->_finish, _alloc);
                    this->_finish = new_finish;
                    std::move_backward(pos, nfinish, old_finish);
                    std::move(first, last, pos);
                }
                else
                {
                    _ForwardIter mid = first;
                    std::advance(mid, elms_after);
                    iterator npos = __uninit_copy_with_allocator(
                        mid, last, this->_finish, _alloc);
                    __uninit_move_with_allocator(pos, this->_finish, npos,
                                                 _alloc);
                    this->_finish = new_finish;
                    std::copy(first, mid, pos);
                }
            }
            catch (...)
            {
                for (map_pointer mcurr = this->_finish._node + 1;
                     mcurr < this->_finish._node + 1; ++mcurr)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
            }
        }
    }

    void
    _insert_aux(iterator pos, size_type n, const value_type &x)
    {
        const difference_type elms_before = pos - this->_start;
        const size_type length            = this->size();

        if (elms_before < difference_type(length / 2))
        {
            iterator new_start = _reserve_elements_at_front(n);
            iterator old_start = this->_start;
            pos                = this->_start + elms_before;

            try
            {
                if (elms_before >= difference_type(n))
                {
                    iterator nstart = this->_start + difference_type(n);
                    __uninit_move_with_allocator(this->_start, nstart,
                                                 new_start, _alloc);
                    this->_start = new_start;
                    std::move(nstart, pos, old_start);
                    std::fill(pos - difference_type(n), pos, x);
                }
                else
                {
                    iterator mid = __uninit_move_with_allocator(
                        this->_start, pos, new_start, _alloc);

                    try
                    {
                        __uninit_fill_with_allocator(mid, this->_start, x,
                                                     _alloc);
                    }
                    catch (...)
                    {
                        __destroy_range(new_start, mid, _alloc);
                        throw;
                    }

                    this->_start = new_start;
                    std::fill(old_start, pos, x);
                }
            }
            catch (...)
            {
                for (map_pointer mcurr = new_start._node;
                     mcurr < this->_start._node; ++mcurr)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
        else
        {
            iterator new_finish = _reserve_elements_at_back(n);
            iterator old_finish = this->_finish;
            const difference_type elms_after =
                difference_type(length) - elms_before;
            pos = this->_finish - elms_after;

            try
            {
                if (elms_after > difference_type(n))
                {
                    iterator nfinish = (this->_finish - difference_type(n));
                    __uninit_move_with_allocator(nfinish, this->_finish,
                                                 this->_finish, _alloc);
                    this->_finish = new_finish;
                    std::move(pos, nfinish, old_finish);
                    std::fill(pos, pos + difference_type(n), x);
                }
                else
                {
                    __uninit_fill_with_allocator(
                        this->_finish, pos + difference_type(n), x, _alloc);

                    try
                    {
                        __uninit_move_with_allocator(pos, this->_finish,
                                                     pos + difference_type(n),
                                                     _alloc);
                    }
                    catch (...)
                    {
                        __destroy_range(this->_finish, pos + difference_type(n),
                                        _alloc);
                        throw;
                    }

                    this->_finish = new_finish;
                    std::fill(pos, old_finish, x);
                }
            }
            catch (...)
            {
                for (map_pointer mcurr = this->_finish._node + 1;
                     mcurr < new_finish._node + 1; ++mcurr)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
    }

    void
    _fill_insert_aux(iterator pos, size_type n, const value_type &x)
    {
        if (pos._curr == this->_start._curr)
        {
            iterator new_start = _reserve_elements_at_front(n);
            try
            {
                __uninit_fill_with_allocator(new_start, this->_start, x,
                                             _alloc);
                this->_start = new_start;
            }
            catch (...)
            {
                for (map_pointer mcurr = new_start._node;
                     mcurr < this->_start._node; mcurr++)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
        else if (pos._curr == this->_start._curr)
        {
            iterator new_finish = _reserve_elements_at_back(n);
            try
            {
                __uninit_fill_with_allocator(this->_finish, new_finish, n,
                                             _alloc);
                this->_finish = new_finish;
            }
            catch (...)
            {
                for (map_pointer mcurr = new_finish._node;
                     mcurr < this->_finish._node; ++mcurr)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
        else
            _insert_aux(pos, n, x);
    }

    template <typename _InputIter>
    void
    _range_insert_aux(iterator pos, _InputIter first, _InputIter last,
                      std::forward_iterator_tag)
    {
        const size_type nodes_to_add = std::distance(first, last);

        if (pos._curr == this->_start._curr)
        {
            iterator new_start = _reserve_elements_at_front(nodes_to_add);
            try
            {
                __uninit_copy_with_allocator(first, last, new_start, _alloc);
                this->_start = new_start;
            }
            catch (...)
            {
                for (map_pointer mcurr = new_start._node;
                     mcurr < this->_start._node; mcurr++)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
        else if (pos._curr == this->_finish._curr)
        {
            iterator new_finish = _reserve_elements_at_back(nodes_to_add);
            try
            {
                __uninit_copy_with_allocator(first, last, this->_finish,
                                             _alloc);
                this->_finish = new_finish;
            }
            catch (...)
            {
                for (map_pointer mcurr = this->_finish._node + 1;
                     new_finish._node + 1; mcurr++)
                    _Tp_alloc_traits::deallocate(
                        _alloc, std::addressof(**mcurr), _max_nodes());
                throw;
            }
        }
        else
            _insert_aux(pos, first, last, nodes_to_add);
    }

    void
    _pop_front_aux()
    {
        _Tp_alloc_traits::destroy(_alloc, this->_start._curr);
        _Tp_alloc_traits::deallocate(_alloc, this->_start._first, _max_nodes());
        this->_start.set_node(this->_start._node + 1);
        this->_start._curr = this->_start._first;
    }

    void
    _pop_back_aux()
    {
        _Tp_alloc_traits::deallocate(_alloc, this->_finish._first,
                                     _max_nodes());
        this->_finish.set_node(this->_finish._node - 1);
        this->_finish._curr = this->_finish._last - 1;
        _Tp_alloc_traits::destroy(_alloc, this->_finish._curr);
    }

    iterator
    _erase(iterator pos)
    {
        iterator next = pos;
        ++next;
        const difference_type index = pos - begin();

        if (static_cast<size_type>(index) < (this->size() / 2))
        {
            if (pos != begin())
                std::move_backward(begin(), pos, next);
            pop_front();
        }
        else
        {
            if (next != end())
                std::move(next, end(), pos);
            pop_back();
        }

        return begin() + index;
    }

    void
    _erase_from_begin(iterator end)
    {
        _destroy_data(begin(), end);
        _deallocate_nodes(this->_start._node, end._node);
        this->_start = end;
    }

    void
    _erase_to_end(iterator start)
    {
        _destroy_data(start, end());
        _deallocate_nodes(start._node + 1, this->_finish._node + 1);
        this->_finish = start;
    }

    iterator
    _erase_range(iterator first, iterator last)
    {
        if (first == last)
            return first;
        else if (first == begin() && last == end())
        {
            clear();
            return end();
        }

        const difference_type n           = last - first;
        const difference_type elms_before = first - begin();
        if (static_cast<size_type>(elms_before) <= (size() - n) / 2)
        {
            if (first != begin())
                std::move_backward(begin(), first, last);
            _erase_from_begin(begin() + n);
        }
        else
        {
            if (last != end())
                std::move(last, end(), first);
            _erase_to_end(end() - n);
        }

        return begin() + elms_before;
    }
};
} // namespace opendsa

#endif
