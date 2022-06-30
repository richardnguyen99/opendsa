/**
 * @file deque.h
 * @author Richard Nguyen (richard.ng0616@gmail.com)
 * @brief An oversimplified implementation of deque
 * @see docs/deque.h
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

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

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
        deque_iterator() noexcept
            : _curr(item_pointer()), _first(item_pointer()),
              _last(item_pointer()), _map(map_pointer())
        {
        }

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

        deque_iterator &operator++() noexcept
        {
            ++_curr;
            if (_curr == _last)
            {
                this->set_node(_map + 1);
                _curr = _first;
            }

            return *this;
        }

        deque_iterator operator++(int) noexcept
        {
            deque_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        deque_iterator &operator--() noexcept
        {
            if (_curr == _first)
            {
                this->set_node(_map - 1);
                _curr = _last;
            }
            --_curr;
            return *this;
        }

        deque_iterator operator--(int) noexcept
        {
            deque_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        deque_iterator &operator+=(difference_type n) noexcept
        {
            const difference_type offset = n + (_curr - _first);
            if (offset >= 0 && offset < difference_type(get_node_elements()))
                _curr += n;
            else
            {
                const difference_type node_offset
                    = offset > 0 ? offset / difference_type(get_node_elements())
                                 : -difference_type((-offset - 1)
                                                    / get_node_elements())
                                       - 1;
                this->set_node(_map + node_offset);
                _curr = _first + (offset - node_offset)
                        + difference_type(get_node_elements());
            }

            return *this;
        }

        deque_iterator operator+(difference_type n) const noexcept
        {
            deque_iterator tmp = *this;
            return tmp += n;
        }

        deque_iterator &operator-=(difference_type n) noexcept
        {
            return *this += -n;
        }

        deque_iterator operator-(difference_type n) const noexcept
        {
            deque_iterator tmp = *this;
            return tmp -= n;
        }

        void set_node(map_pointer new_node) noexcept
        {
            this->_map   = new_node;
            this->_first = *new_node;
            this->_last  = this->_first + difference_type(get_node_elements());
        }
    };

    template <typename _Tp, typename _Ref, typename _Ptr>
    inline bool operator==(const deque_iterator<_Tp, _Ref, _Ptr> &lhs,
                           const deque_iterator<_Tp, _Ref, _Ptr> &rhs) noexcept
    {
        return rhs._curr == lhs._curr;
    }

    template <typename _Tp, typename _Ref, typename _Ptr>
    inline bool operator!=(const deque_iterator<_Tp, _Ref, _Ptr> &lhs,
                           const deque_iterator<_Tp, _Ref, _Ptr> &rhs) noexcept
    {
        return rhs._curr != lhs._curr;
    }

    template <typename _Tp, typename _Ref, typename _Ptr>
    inline typename deque_iterator<_Tp, _Ref, _Ptr>::difference_type
    operator-(const deque_iterator<_Tp, _Ref, _Ptr> &lhs,
              const deque_iterator<_Tp, _Ref, _Ptr> &rhs) noexcept
    {
        using iterator        = deque_iterator<_Tp, _Ref, _Ptr>;
        using difference_type = typename iterator::difference_type;

        // Calculate how far two iterators
        const difference_type diff_on_node
            = iterator::get_node_elements() * (lhs._map - rhs._map - 1);

        const difference_type diff_on_lhs = (lhs._curr - rhs._first);
        const difference_type diff_on_rhs = (rhs._last - rhs._curr);

        return diff_on_node + diff_on_lhs + diff_on_rhs;
    }

    template <typename _Tp, typename _Ref1, typename _Ptr1, typename _Ref2,
              typename _Ptr2>
    inline typename deque_iterator<_Tp, _Ref1, _Ptr1>::difference_type
    operator-(const deque_iterator<_Tp, _Ref1, _Ptr1> &lhs,
              const deque_iterator<_Tp, _Ref2, _Ptr2> &rhs)
    {
        using iterator        = deque_iterator<_Tp, _Ref1, _Ptr1>;
        using difference_type = typename iterator::difference_type;

        // Calculate how far two iterators
        const difference_type diff_on_node
            = iterator::get_node_elements() * (lhs._map - rhs._map - 1);

        const difference_type diff_on_lhs = (lhs._curr - rhs._first);
        const difference_type diff_on_rhs = (rhs._last - rhs._curr);

        return diff_on_node + diff_on_lhs + diff_on_rhs;
    }

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
        using const_iterator   = deque_iterator<_Tp, const _Tp &, _Ptr_const>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using value_type      = _Tp;
        using pointer         = _Ptr;
        using const_pointer   = _Ptr_const;
        using reference       = _Tp &;
        using const_reference = const _Tp &;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        /**
         * @brief Creates an empty %deque.
         */
        constexpr deque()
            : _map(), _map_size(0), _start(), _finish(), _alloc(), _map_alloc()
        {
            _initialize_map(0);
        }

        /**
         * @brief Creates a deque filled with @a n default values of @a _Tp.
         *
         * @param n The number of elements to initialize.
         *
         * This constructor fills the %deque with @a n default constructed
         * elements of type @a _Tp. A default constructed element can be the
         * default value or called via default constructor. Thus, if _Tp is a
         * user-defined class, a default constructor should be supported.
         */
        explicit deque(size_type n)
            : _map(), _map_size(n), _start(), _finish(), _alloc(), _map_alloc()
        {
            _initialize_map(n);
            _default_construct();
        }

        /**
         * @brief Creates a deque filled with @a n copies of @a value.
         *
         * @param n The number of elements to initialize.
         * @param value An element to create copies.
         *
         * This constructor fills the %deque with @a n copies of @a value. If @n
         * value is a user-defined type, a copy constructor should be available.
         */
        constexpr deque(size_type n, const value_type &value)
            : _map(), _map_size(n), _start(), _finish(), _alloc(), _map_alloc()
        {
            _initialize_map(n);
            _fill_construct(value);
#ifdef DEBUG
            std::cout << "Deque's count copyies constructor is called\n";
            std::cout << "New deque diff: " << this->size() << "\n";
            std::for_each(this->_start, this->_finish,
                          [](const auto &curr) { std::cout << curr << " "; });
            std::cout << "\n\n";
#endif
        }

        /**
         * @brief  Creates a deque by copying another.
         *
         * @param other Another %deque of same type and allocator.
         *
         * This constructor creates the new %deque by filling each element with
         * the according element from @a other.
         */
        constexpr deque(const deque &other)
            : _map(), _map_size(other._map_size), _start(), _finish(), _alloc(),
              _map_alloc()
        {
            _initialize_map(other.size());
            std::__uninitialized_copy_a(other.cbegin(), other.cend(), _start,
                                        _alloc);

#ifdef DEBUG
            std::cout << "Deque's copy constructor is called\n";
            std::cout << "Other deque diff: " << (other.cend() - other.cbegin())
                      << "\n";
            std::for_each(other.cbegin(), other.cend(),
                          [](const auto &curr) { std::cout << curr << " "; });
            std::cout << "\n";
            std::cout << "New deque diff: " << (this->_finish - this->_start)
                      << "\n";
            std::for_each(this->_start, this->_finish,
                          [](const auto &curr) { std::cout << curr << " "; });
            std::cout << "\n\n";
#endif
        }

        /**
         * @brief  Creates a deque by moving another.
         *
         * This constructor creates the new %deque by moving each element from
         * %other to the new one.
         */
        constexpr deque(deque &&other)
            : _map(), _map_size(other._map_size), _start(other._start),
              _finish(other._finish), _alloc(), _map_alloc()
        {

            other._start    = iterator();
            other._finish   = iterator();
            other._map      = _Map_ptr();
            other._map_size = 0;

#ifdef DEBUG
            std::cout << "Deque's move constructor is called\n";
            std::cout << "Other deque diff: " << other.size() << "\n";
            std::for_each(other.cbegin(), other.cend(),
                          [](const auto &curr) { std::cout << curr << " "; });
            std::cout << "New deque diff: " << this->size() << "\n";
            std::for_each(this->cbegin(), this->cend(),
                          [](const auto &curr) { std::cout << curr << " "; });
            std::cout << "\n\n";
#endif
        }

        // Iterators

        /**
         * @brief Gets a read/write iterator pointing to the first element in
         * %deque.
         */
        constexpr iterator begin() noexcept { return this->_start; }

        /**
         * @brief  Gets a read-only iterator pointing to the first element in
         * %deque.
         */
        constexpr const_iterator cbegin() const noexcept
        {
            return this->_start;
        }

        /**
         * @brief Gets a read/write iterator pointing to the end of %deque (not
         * the last element).
         */
        constexpr iterator end() noexcept { return this->_finish; }

        /**
         * @brief  Gets a read-only iterator pointing to the end of %deque (not
         * the last element)
         */
        constexpr const_iterator cend() const noexcept { return this->_finish; }

        /**
         * @brief  Gets a read/write reverse iterator pointing to the last
         * element of %deque.
         */
        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(this->_finish);
        }

        /**
         * @brief  Gets a read-only reverse iterator pointing to the last
         * element of %deque.
         */
        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(this->_finish);
        }

        /**
         * @brief Gets a read/write reverse iterator pointing to the beginning
         * of %deque (not the first element).
         */
        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator(this->_start);
        }

        /**
         * @brief Gets a read-only reverse iterator pointing to the beginning
         * of %deque (not the first element).
         */
        constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(this->_finish);
        }

        // Capacity
        /**
         * @brief Returns the number of elements in the container.
         *
         * @return constexpr size_type
         */
        constexpr size_type size() const noexcept
        {
            return std::max(this->_finish - this->_start, difference_type(0));
        }

        constexpr bool empty() const noexcept
        {
            return this->_finish == this->_start;
        }

        constexpr bool max_size() const noexcept
        {
            return _Tp_alloc_traits::max_size(_alloc);
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

        void _default_construct()
        {
            _Map_ptr curr;
            try
            {
                for (curr = this->_start._map; curr < this->_finish._map;
                     ++curr)
                    std::__uninitialized_default_a(
                        *curr, *curr + iterator::get_node_elements(), _alloc);
                std::__uninitialized_default_a(this->_finish._first,
                                               this->_finish._curr, _alloc);
            }
            catch (...)
            {
                std::_Destroy(this->_start, iterator(*curr, curr), _alloc);
                throw;
            }
        }

        void _fill_construct(const value_type &value)
        {
            _Map_ptr curr;
            try
            {
                for (curr = this->_start._map; curr < this->_finish._map;
                     ++curr)
                    std::__uninitialized_fill_a(
                        *curr, *curr + iterator::get_node_elements(), value,
                        _alloc);
                std::__uninitialized_fill_a(this->_finish._first,
                                            this->_finish._curr, value, _alloc);
            }
            catch (...)
            {
                std::_Destroy(this->_start, iterator(*curr, curr), _alloc);
                throw;
            }
        }

        void _copy_construct(iterator first, iterator last) {}
    };
} // namespace opendsa

#endif
