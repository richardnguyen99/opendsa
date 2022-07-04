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
    constexpr inline std::size_t get_deque_buffer_size(std::size_t size_of_type)
    {
        std::size_t DEQUE_DEFAULT_BUFFER_SIZE = 512;
        return (size_of_type < DEQUE_DEFAULT_BUFFER_SIZE)
                   ? std::size_t(DEQUE_DEFAULT_BUFFER_SIZE / size_of_type)
                   : std::size_t(1);
    }

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

        node_pointer _curr;
        node_pointer _first;
        node_pointer _last;
        map_pointer  _node;

        static size_type get_nnodes() noexcept
        {
            return get_deque_buffer_size(sizeof(value_type));
        }

        deque_iterator() noexcept : _curr(), _first(), _last(), _node() {}

        deque_iterator(node_pointer nptr, map_pointer mptr) noexcept
            : _curr(nptr), _first(*mptr), _last(*mptr + get_nnodes()),
              _node(mptr)
        {
        }

        template <typename _Iter,
                  typename = typename std::enable_if<std::conjunction<
                      std::is_same<deque_iterator, const_iterator>,
                      std::is_same<_Iter, iterator>>::value>::type>
        deque_iterator(const _Iter &__x) noexcept
            : _curr(__x._curr), _first(__x._first), _last(__x._last),
              _node(__x._node)
        {
        }

        deque_iterator(const deque_iterator &other) noexcept
            : _curr(other._curr), _first(other._first), _last(other._last),
              _node(other._node)
        {
        }

        deque_iterator &operator=(const deque_iterator &) = default;

        reference operator*() const noexcept { return *_curr; }

        pointer operator->() const noexcept { return _curr; }

        deque_iterator &operator++() noexcept
        {
            ++_curr;
            if (_curr == _last)
            {
                set_node(_node + 1);
                _curr = _first;
            }

            return *this;
        }

        deque_iterator operator++(int) const noexcept
        {
            deque_iterator temp = *this;
            ++*this;
            return temp;
        }

        deque_iterator &operator--() noexcept
        {
            if (_curr == _first)
            {
                set_node(_node - 1);
                _curr = _last;
            }
            --_curr;
            return *this;
        }

        deque_iterator operator--(int) const noexcept
        {
            deque_iterator temp = *this;
            ++*this;
            return temp;
        }

        deque_iterator &operator+=(difference_type n) noexcept
        {
            // This overloaded operator accepts both positive and negative
            // values to traverse the %deque_iterator. Therefore, every
            // check should include both signs.
            const difference_type elm_offset = n + (_curr - _first);

            if (elm_offset >= 0 && elm_offset < difference_type(get_nnodes()))
                _curr += n;
            else
            {
                const difference_type node_offset
                    = elm_offset > 0
                          ? elm_offset / difference_type(get_nnodes())
                          : -difference_type((-elm_offset - 1) / get_nnodes())
                                - 1;
                set_node(_node + node_offset);
                _curr = _first
                        + (elm_offset
                           - node_offset * difference_type(get_nnodes()));
            }
        }

        deque_iterator &operator-=(difference_type n) noexcept
        {
            return (*this += -n);
        }

        reference operator[](difference_type n) const noexcept
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
        void set_node(map_pointer node) noexcept
        {
            _node  = node;
            _first = *node;
            _last  = _first + get_nnodes();
        }

        friend bool operator==(const deque_iterator &lhs,
                               const deque_iterator &rhs) noexcept
        {
            return lhs._curr == rhs._curr;
        }

        friend bool operator!=(const deque_iterator &lhs,
                               const deque_iterator &rhs) noexcept
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const deque_iterator &lhs,
                              const deque_iterator &rhs) noexcept
        {
            return (lhs._node == rhs._node) ? (lhs._curr < rhs._curr)
                                            : (lhs._node < rhs._node);
        }

        friend bool operator>(const deque_iterator &lhs,
                              const deque_iterator &rhs) noexcept
        {
            return rhs < lhs;
        }

        friend bool operator<=(const deque_iterator &lhs,
                               const deque_iterator &rhs) noexcept
        {
            return !(rhs < lhs);
        }

        friend bool operator>=(const deque_iterator &lhs,
                               const deque_iterator &rhs) noexcept
        {
            return !(lhs > rhs);
        }

        friend difference_type operator-(const deque_iterator &lhs,
                                         const deque_iterator &rhs) noexcept
        {
            return difference_type(get_nnodes())
                       * (lhs._node - rhs._node - int(lhs._node != 0))
                   + (lhs._curr - lhs._first) + (rhs._last - rhs._curr);
        }

        friend deque_iterator operator+(const deque_iterator &self,
                                        difference_type       n) noexcept
        {
            deque_iterator tmp = self;
            tmp += n;
            return tmp;
        }

        friend deque_iterator operator+(difference_type       n,
                                        const deque_iterator &self) noexcept
        {
            return (self + n);
        }

        friend deque_iterator operator-(const deque_iterator &self,
                                        difference_type       n) noexcept
        {
            deque_iterator tmp = self;
            tmp -= n;
            return tmp;
        }

        friend deque_iterator operator-(difference_type       n,
                                        const deque_iterator &self) noexcept
        {
            return (self - n);
        }
    };

    template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
    class deque
    {
    private:
        using _Tp_alloc_type =
            typename std::allocator_traits<_Alloc>::rebind_alloc<_Tp>;
        using _Tp_alloc_traits = std::allocator_traits<_Tp_alloc_type>;
        using _Tp_ptr          = typename _Tp_alloc_traits::pointer;
        using _Tp_ptr_const    = typename _Tp_alloc_traits::const_pointer;

        using _Map_alloc_type =
            typename std::allocator_traits<_Alloc>::rebind_alloc<_Tp_ptr>;
        using _Map_alloc_traits = std::allocator_traits<_Map_alloc_type>;

    public:
        using value_type      = _Tp;
        using reference       = _Tp &;
        using const_reference = const _Tp &;
        using pointer         = _Tp_ptr;
        using const_pointer   = _Tp_ptr_const;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using iterator        = deque_iterator<_Tp, _Tp &, _Tp_ptr>;
        using const_iterator  = deque_iterator<_Tp, const _Tp &, _Tp_ptr_const>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using node_pointer = typename iterator::node_pointer;
        using map_pointer  = typename iterator::map_pointer;

        /**
         * @brief Creates an empty %deque.
         */
        deque() : _start(), _finish(), _map(), _map_size()
        {
            _initialize_map(0);
        }

        /**
         * @brief Creates a %deque filled with default constructed elements.
         *
         * @param count The number of elements.
         *
         * This constructor creates a deque object by filling it with `n`
         * number of default values of `_Tp`.
         */
        explicit deque(size_type count)
            : _start(), _finish(), _map(), _map_size()
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
        deque(size_type count, const value_type &value)
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
        template <
            typename _InputIter,
            typename = typename std::enable_if<std::is_convertible<
                typename std::iterator_traits<_InputIter>::iterator_category,
                std::input_iterator_tag>::value>::type>
        deque(_InputIter first, _InputIter last)
        {
            typename std::iterator_traits<_InputIter>::iterator_category
                iter_traits
                = typename std::iterator_traits<
                    _InputIter>::iterator_category();

            _range_construct(first, last, iter_traits);
        }

        /**
         * @brief Creates a %deque based on a initializer list.
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

        deque(const deque &other)
        {
            _range_construct(other.cbegin(), other.cend(),
                             std::random_access_iterator_tag());
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
            _deallocate_nodes(this->_start._node, this->_finish._node);
            _deallocate_map(this->_map, this->_map_size);
        }

        iterator begin() noexcept { return this->_start; }

        const_iterator cbegin() const noexcept { return this->_start; }

        iterator end() noexcept { return this->_finish; }

        const_iterator cend() const noexcept { return this->_finish; }

        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(this->_finish);
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(this->_finish);
        }

        reverse_iterator rend() noexcept
        {
            return reverse_iterator(this->_start);
        }

        const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(this->_start);
        }

    private:
        constexpr static size_type INITIAL_MAP_SIZE = 8;

        iterator        _start;
        iterator        _finish;
        map_pointer     _map;
        size_type       _map_size;
        _Tp_alloc_type  _alloc;
        _Map_alloc_type _map_alloc;

        static size_type _max_nodes()
        {
            return get_deque_buffer_size(sizeof(value_type));
        }

        void _destroy_data(iterator first, iterator last)
        {
            for (map_pointer curr = first._node + 1; curr < last._node; curr++)
            {
                for (node_pointer node_curr = *curr;
                     node_curr < *curr + _max_nodes(); node_curr++)
                    _Tp_alloc_traits::destroy(_alloc,
                                              std::addressof(*node_curr));
            }

            if (first._node != last._node)
            {
                for (node_pointer curr = first._curr; curr != first._last;
                     curr++)
                    _Tp_alloc_traits::destroy(_alloc, std::addressof(*curr));
                for (node_pointer curr = last._first; curr != last._curr;
                     curr++)
                    _Tp_alloc_traits::destroy(_alloc, std::addressof(*curr));
            }
            else
            {
                for (node_pointer curr = first._curr; curr != last._curr + 1;
                     curr++)
                    _Tp_alloc_traits::destroy(_alloc, std::addressof(*curr));
            }
        }

        void _deallocate_nodes(map_pointer first, map_pointer last)
        {
            for (map_pointer curr = first; curr != last + 1; curr++)
                _Tp_alloc_traits::deallocate(_alloc, std::addressof(**curr),
                                             _max_nodes());
        }

        void _deallocate_map(map_pointer map, size_type map_size)
        {
            _Map_alloc_traits::deallocate(_map_alloc, this->_map,
                                          this->_map_size);
        }

        void _create_nodes(map_pointer map_start, map_pointer map_finish)
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
        void _initialize_map(size_type num_elms)
        {
            const size_type num_nodes = (num_elms / _max_nodes() + 1);

            this->_map_size
                = std::max(INITIAL_MAP_SIZE, size_type(num_nodes + 2));
            this->_map
                = _Map_alloc_traits::allocate(_map_alloc, this->_map_size);

            for (size_type i = 0; i < this->_map_size; i++)
                this->_map[i] = nullptr;

            map_pointer map_start
                = this->_map + (this->_map_size - num_nodes) / 2;
            map_pointer map_finish = map_start + num_nodes;

            try
            {
                _create_nodes(map_start, map_finish);
            }
            catch (...)
            {
                _deallocate_map(this->_map, this->_map_size);
                throw;
            }

            this->_start.set_node(map_start);
            this->_finish.set_node(map_finish - 1);
            this->_start._curr = this->_start._first;
            this->_finish._curr
                = this->_finish._first + num_elms % _max_nodes();
        }

        void _fill_construct(const value_type &value)
        {
            map_pointer curr;
            try
            {
                for (curr = this->_start._node; curr < this->_finish._node;
                     ++curr)
                {
                    for (node_pointer node_curr = *curr;
                         node_curr < *curr + _max_nodes(); node_curr++)
                        _Tp_alloc_traits::construct(
                            _alloc, std::addressof(*node_curr), value);
                }

                for (node_pointer node_curr = this->_finish._first;
                     node_curr < this->_finish._curr; node_curr++)
                    _Tp_alloc_traits::construct(
                        _alloc, std::addressof(*node_curr), value);
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
        void _range_construct(_InputIter first, _InputIter last,
                              std::forward_iterator_tag)
        {
            const size_type n = std::distance(first, last);
            this->_initialize_map(n);

            map_pointer curr;
            try
            {
                for (curr = this->_start._node; curr < this->_finish._node;
                     ++curr)
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
                    _Tp_alloc_traits::construct(
                        _alloc, std::addressof(*(*curr + i)), *tmp);
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
    };
} // namespace opendsa

#endif
