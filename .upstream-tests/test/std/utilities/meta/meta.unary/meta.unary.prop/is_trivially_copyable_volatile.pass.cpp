//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_trivially_copyable for volatile qualified types

// These compilers have not implemented Core 2094 which makes volatile
// qualified types trivially copyable.
// XFAIL: clang-3, clang-4, apple-clang-6, apple-clang-7, apple-clang-8, apple-clang-9.0, gcc, icc, pgi, msvc

// Note that this passes under NVRTC, so don't mark this as expected failure for NVCC,  because that'll get us
// the wrong result under runtime compilation (since NVRTC also has the `nvcc` feature).
// (It's possible that the compiler detection should be changed to not report `nvcc` for NVRTC, but that's a
// topic for another day.)

// If we're just building the test and not executing it, it should pass.
// UNSUPPORTED: no_execute

#include <type_traits>
#include <cassert>
#include "test_macros.h"

template <class T>
__host__ __device__
void test_is_trivially_copyable_volatile()
{
    static_assert( std::is_trivially_copyable<volatile T>::value, "");
    static_assert( std::is_trivially_copyable<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_trivially_copyable_v<volatile T>, "");
    static_assert( std::is_trivially_copyable_v<const volatile T>, "");
#endif
}

template <class T>
__host__ __device__
void test_is_not_trivially_copyable_volatile()
{
    static_assert(!std::is_trivially_copyable<volatile T>::value, "");
    static_assert(!std::is_trivially_copyable<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_trivially_copyable_v<volatile T>, "");
    static_assert(!std::is_trivially_copyable_v<const volatile T>, "");
#endif
}

struct A
{
    int i_;
};

struct B
{
    int i_;
    ~B() {assert(i_ == 0);}
};

class C
{
public:
    C();
};

int main(int, char**)
{
    test_is_trivially_copyable_volatile<int> ();
    test_is_trivially_copyable_volatile<const int> ();
    test_is_trivially_copyable_volatile<A> ();
    test_is_trivially_copyable_volatile<const A> ();
    test_is_trivially_copyable_volatile<C> ();

    test_is_not_trivially_copyable_volatile<int&> ();
    test_is_not_trivially_copyable_volatile<const A&> ();
    test_is_not_trivially_copyable_volatile<B> ();

    return 0;
}
