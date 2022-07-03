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
#include <iterator>
#include <memory>

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
        using _iter_template
            = deque_iterator<_Tp, _CvTp,
                             typename std::pointer_traits<_Ptr>::rebind<_CvTp>>;

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

        deque_iterator() noexcept : _curr(), _first(), _last(), _node()
        {
#ifdef DEBUG
#ifdef ITER
            std::cout
                << "======== deque_iterator default constructor is called\n";
            std::cout << "_curr: " << _curr
                      << ""
                         "\n";
            std::cout << "_curr of type " << typeid(_curr).name() << ": "
                      << _curr << "\n";
            std::cout << "_first of type " << typeid(_first).name() << ": "
                      << _first << "\n";
            std::cout << "_last of type " << typeid(_last).name() << ": "
                      << _last << "\n";
            std::cout << "_node of type " << typeid(_node).name() << ": "
                      << _node << "\n";
#endif
#endif
        }

        static size_type get_nnodes() noexcept
        {
            return get_deque_buffer_size(sizeof(value_type));
        }

        deque_iterator(node_pointer nptr, map_pointer mptr) noexcept
            : _curr(nptr), _first(*mptr), _last(*mptr + get_nnodes()),
              _node(mptr)
        {
#ifdef DEBUG
#ifdef ITER
            std::cout << "======== deque_iterator constructor is called\n";
            std::cout << "_curr: " << _curr << "\n";
            std::cout << "_curr of type " << typeid(_curr).name() << ": "
                      << _curr << "\n";
            std::cout << "_first of type " << typeid(_first).name() << ": "
                      << _first << "\n";
            std::cout << "_last of type " << typeid(_last).name() << ": "
                      << _last << "\n";
            std::cout << "_node of type " << typeid(_node).name() << ": "
                      << _node << "\n";
#endif
#endif
        }

        /**
         * @brief Mainly control traversal amongst nodes in the map
         *
         * @param node An existing node in the map
         *
         * This function will control what node in the map is held by this
         * %iterator. The information is the address of the node itself, and the
         * capacity. However, how many internal elements, i.e. the bound aka @a
         * _curr, is defined by the caller method.
         */
        void set_node(map_pointer node) noexcept
        {
            _node  = node;
            _first = *node;
            _last  = _first + get_nnodes();
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

        using node_pointer = typename iterator::node_pointer;
        using map_pointer  = typename iterator::map_pointer;

        /**
         * @brief Creates an empty %deque
         */
        deque() : _start(), _finish(), _map(), _map_size()
        {
            _initialize_map(0);
        }

        /**
         * @brief Destructor erases elements and reclaims memory.
         *
         *  The dtor only erases the elements, and note that if the elements
         *  themselves are pointers, the pointed-to memory is not touched in any
         *  way.  Managing the pointer is the user's responsibility.
         */
        ~deque()
        {
            _destroy_data(this->_start, this->_finish);
            _deallocate_nodes(this->_start._node, this->_finish._node);
            _deallocate_map(this->_map, this->_map_size);
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
         * This helper function initializes a doubly-ended queue that contains
         * an array of pointers. Each pointer holds an address of a fixed-size
         * array of elements where the size is determined over the size of each
         * element.
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
    };
} // namespace opendsa

#endif
