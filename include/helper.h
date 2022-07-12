#ifndef __OPENDSA_HELPER_H
#define __OPENDSA_HELPER_H 1

#include <algorithm>
#include <iostream>
#include <memory>

#ifndef NDEBUG
#define M_Assert(Expr, Msg) __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#define M_Assert(Expr, Msg) ;
#endif

void __M_Assert(const char *expr_str, bool expr, const char *file, int line,
                const char *msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << " = true\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

namespace opendsa
{
    /**
     * @brief Destroys objects in range [first, last).
     */
    template <typename _ForwardIter, typename _Allocator>
    void __destroy_range(_ForwardIter __first, _ForwardIter __last,
                         _Allocator &__alloc)
    {
        using traits_t = std::allocator_traits<_Allocator>;
        for (; __first != __last; ++__first)
            traits_t::destroy(__alloc, std::addressof(*__first));
    }

    /**
     * @brief Initializes [first, last) to starting pointer __start_result.
     */
    template <typename _InputIter, typename _ForwardIter, typename _Allocator>
    _ForwardIter __uninit_copy_with_allocator(_InputIter   __first,
                                              _InputIter   __last,
                                              _ForwardIter __start_result,
                                              _Allocator  &__alloc)
    {
        _ForwardIter __curr = __start_result;

        try
        {
            using traits_t = std::allocator_traits<_Allocator>;
            for (; __first != __last; ++__first, ++__curr)
                traits_t::construct(__alloc, std::addressof(*__curr), *__first);
        }
        catch (...)
        {
            __destroy_range(__start_result, __curr, __alloc);
            throw;
        }

        return __curr;
    }

    /**
     * @brief Moves [first, last) to starting pointer start_result.
     */
    template <typename _InputIter, typename _ForwardIter, typename _Allocator>
    _ForwardIter __uninit_move_with_allocator(_InputIter   __first,
                                              _InputIter   __last,
                                              _ForwardIter __start_result,
                                              _Allocator  &__alloc)
    {
        return __uninit_copy_with_allocator(std::make_move_iterator(__first),
                                            std::make_move_iterator(__last),
                                            __start_result, __alloc);
    }
} // namespace opendsa

#ifndef NDEBUG
// For deque
#define _NON_EMPTY_DEQUE(Msg) M_Assert(!this->empty(), Msg)
#endif
#endif // __OPENDSA__HELPER_H
