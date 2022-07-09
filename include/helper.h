#ifndef __OPENDSA_HELPER_H
#define __OPENDSA_HELPER_H 1

#include <algorithm>
#include <iostream>

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

#ifndef NDEBUG
// For deque
#define _NON_EMPTY_DEQUE(Msg) M_Assert(!this->empty(), Msg)
#endif
#endif // __OPENDSA__HELPER_H
