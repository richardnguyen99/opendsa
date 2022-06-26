/**
 * @file deque.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief An oversimplified implementation of dequeu
 * @version 0.1
 * @date 2022-06-25
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __OPENDSA_DEQUE_H
#define __OPENDSA_DEQUE_H 1

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>

namespace opendsa
{
    /**
     * @brief Iterator to control two ends of a deque
     *
     * A deque class will hold two internal %deque_iterator objects to mark the
     * valid range. Each deque iterator behaves like an array of fixed elements,
     * which is determined by `get_buffer_size()`. Additionally, the deque
     * iterators will keep track of both ends of a deque.
     */
    template <typename _Tp, typename _Ref, typename _Ptr>
    struct deque_iterator
    {
        using iterator       = deque_iterator<_Tp, _Tp &, _Tp *>;
        using const_iterator = deque_iterator<_Tp, const _Tp &, const _Tp *>;
        using item_pointer   = _Tp *;
        using map_pointer    = _Tp **;

        using iterator_category = std::random_access_iterator_tag;
        using value_type        = _Tp;
        using pointer           = _Ptr;
        using reference         = _Ref;
        using size_type         = std::size_t;
        using difference_type   = std::ptrdiff_t;

        item_pointer _curr;
        item_pointer _first;
        item_pointer _last;
        map_pointer  _map;

        /**
         * @brief Compute how many elements in a node.
         */
        constexpr static inline std::size_t get_node_elements() noexcept
        {
            const size_type size = sizeof(_Tp);
            return (size < 512 ? size_type(512 / size) : size_type(1));
        }

        /**
         * @brief Construct an empty deque_iterator object
         */
        deque_iterator() noexcept : _curr(), _first(), _last(), _map() {}

        /**
         * @brief Construct a new deque_iterator object
         */
        deque_iterator(item_pointer i, map_pointer m) noexcept
            : _curr(i), _first(*m), _last(*m + get_node_elements()), _map(m)
        {
        }

        /**
         * @brief Construct a const deque_iterator object
         */
        deque_iterator(const iterator &i) noexcept
            : _curr(i._curr), _first(i._first), _last(i._last), _map(i._map)
        {
        }

        reference operator*() const noexcept { return *_curr; }

        pointer operator->() const noexcept { return _curr; }

        void set_node(map_pointer new_node) noexcept
        {
            this->_map   = new_node;
            this->_first = *new_node;
            this->_last  = this->_first + difference_type(get_node_elements());
        }
    };

    template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
    class deque
    {
    private:
        using _Tp_alloc_traits = std::allocator_traits<_Alloc>;
        using _Ptr             = typename _Tp_alloc_traits::pointer;
        using _Ptr_const       = typename _Tp_alloc_traits::const_pointer;

        using _Map_alloc =
            typename _Tp_alloc_traits::template rebind_alloc<_Ptr>;
        using _Map_alloc_traits = std::allocator_traits<_Map_alloc>;
        using _Map_ptr = typename deque_iterator<_Tp, _Tp &, _Ptr>::map_pointer;

    public:
        using value_allocator_type = _Alloc;
        using map_allocator_type   = _Map_alloc;
        using iterator             = deque_iterator<_Tp, _Tp &, _Ptr>;
        using const_iterator = deque_iterator<_Tp, const _Tp &, _Ptr_const>;

        using value_type      = _Tp;
        using pointer         = _Ptr;
        using const_pointer   = _Ptr_const;
        using reference       = _Tp &;
        using const_reference = const _Tp &;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        deque()
            : _map(), _map_size(0), _start(), _finish(), _alloc(), _map_alloc()
        {
            _initialize_map(0);
        }

    private:
        _Map_ptr   _map;
        size_type  _map_size;
        iterator   _start;
        iterator   _finish;
        _Alloc     _alloc;
        _Map_alloc _map_alloc;

        _Ptr _allocate_node()
        {
            return _Tp_alloc_traits::allocate(_alloc,
                                              iterator::get_node_elements());
        }

        void _deallocate_node(_Ptr p)
        {
            _Tp_alloc_traits::deallocate(_alloc, p,
                                         iterator::get_node_elements());
        }

        void _destroy_nodes(_Map_ptr start, _Map_ptr finish)
        {
            for (_Map_ptr curr = start; curr < finish; ++curr)
                _deallocate_node(*curr);
        }

        void _create_nodes(_Map_ptr start, _Map_ptr finish)
        {
            _Map_ptr curr;

            try
            {
                for (curr = start; curr < finish; curr++)
                    *curr = _allocate_node();
            }
            catch (...)
            {
                _destroy_nodes(start, curr);
                throw; // throw again so that the caller to this function
                       // catches an exception
            }
        }

        void _deallocate_map(_Map_ptr p, size_type n) noexcept
        {
            _Map_alloc_traits::deallocate(_map_alloc, p, n);
        }

        /**
         * @brief Initialize deque used in constructors
         */
        void _initialize_map(size_type num_elems)
        {
            const size_type num_nodes
                = (num_elems / iterator::get_node_elements() + 1);

            this->_map_size = std::max(size_type(8), size_type(num_nodes + 2));
            this->_map = _Map_alloc_traits::allocate(_map_alloc, num_nodes);

            _Map_ptr m_start = (this->_map + (this->_map_size - num_nodes) / 2);
            _Map_ptr m_finish = m_start + num_nodes;

            try
            {
                _create_nodes(m_start, m_finish);
            }
            catch (...)
            {
                _deallocate_map(this->_map, this->_map_size);
                this->_map      = _Map_ptr();
                this->_map_size = 0;
                throw;
            }

            this->_start.set_node(m_start);
            this->_finish.set_node(m_finish - 1);
            this->_start._curr  = this->_start._first;
            this->_finish._curr = this->_finish._first
                                  + num_elems % iterator::get_node_elements();
        }
    };
} // namespace opendsa

#endif
