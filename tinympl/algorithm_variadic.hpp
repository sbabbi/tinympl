// Copyright (C) 2013, Ennio Barbaro.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://sbabbi.github.io/tinympl for documentation.
//
// You are welcome to contact the author at:
//  enniobarbaro@gmail.com
//

#ifndef TINYMPL_ALGORITHM_VARIADIC_HPP
#define TINYMPL_ALGORITHM_VARIADIC_HPP

#include "bind.hpp"
#include "functional.hpp"
#include "variadic.hpp"

namespace tinympl {
namespace variadic {

/**
 * \defgroup VarAlgs Variadic algorithms
 * Algorithms which operate on variadic templates
 * @{
 */


/**
 * \defgroup VarNonModAlgs Non-modifying sequence operations
 * Algorithms which analyze a sequence without producing an output sequence
 * @{
 */

/**
 * \class all_of
 * \brief Determines whether every element in the sequence satisfies the given 
predicate
 * \param F the predicate, `F<T>::type::value` must be convertible to `bool`
 * \return `all_of<...>::type` is a `std::integral_constant<bool,v>` where `v` 
is true iff all the elements in the sequence satisfy the predicate `F`
 * \sa tinympl::all_of
 */
template< template<class ... T> class F, class ... Args> struct all_of;

/**
 * \class any_of
 * \brief Determines whether any of the elements in the sequence satisfy the 
given predicate
 * \param F the predicate, `F<T>::type::value` must be convertible to `bool`
 * \param Args... the input sequence
 * \return `any_of<...>::type` is a `std::integral_constant<bool,v>` where `v` 
is true iff at least one element in the sequence satisfy the predicate `F`
 * \sa tinympl::any_of
 */
template< template<class ... T> class F, class ... Args> struct any_of;

/**
 * \class none_of
 * \brief Determines whether none of the elements in the sequence satisfy the 
given predicate
 * \param F the predicate, `F<T>::type::value` must be convertible to bool
 * \param Args... the input sequence
 * \return `none_of<...>::type` is a `std::integral_constant<bool,v>` where `v` 
is true iff none of the elements in the sequence satisfy the predicate `F`
 * \sa tinympl::none_of
 */
template< template<class ... T> class F, class ... Args> struct none_of;

template<template<class ... T> class F, class ... Args> struct count_if;

/**
 * \class count
 * \brief Counts the number of elements in a sequence equal to a given one
 * \param T the type to be tested.
 * \param Args... the input sequence
 * \return `count<...>::type` is `std::integral_constant<std::size_t,V>` where 
`V` is the number of elements in the sequence equal to `T`
 * \note The comparison is done with \ref tinympl::equal_to - it can be 
specialized.
 * \sa tinympl::count
 */
template<typename T, class ... Args> 
struct count : count_if< bind<equal_to, arg1, T>::template eval_t, Args ... > 
{};

/**
 * \class count_if
 * \brief Counts the number of elements which satisfy a given predicate
 * \param F The predicate - `F<T>::type::value` shall be convertible to bool
 * \param Args... the input sequence
 * \return `count_if<...>::type` is `std::integral_constant<std::size_t,V>` 
where `V` is the number of elements in the sequence which satisfy the predicate 
`F`.
 * \sa tinympl::count_if
 */

template<template<class ... T> class F, class ... Args> struct find_if;

/**
 * \class find
 * \brief Compute the index of the first element in the sequence which is equal 
to the given type T
 * \param T The type to be tested
 * \param Args... the input sequence
 * \return `find<...>::type` is `std::integral_constant<std::size_t,v>` where 
`v` is the 0-based index of the first element which is equal to `T`. If no such 
element exists, `v` is `size<Sequence>::value`.
 * \note The comparison is done with \ref tinympl::equal_to - it can be 
specialized
 * \sa tinympl::find
 */
template<typename T, class ... Args> 
struct find : find_if< bind<equal_to, arg1, T>::template eval_t, Args ... > {};

/**
 * \class find_if Compute the index of a the first element in the sequence which 
satisfies a given predicate
 * \param F The test predicate - `F<T>::type::value` shall be convertible to 
bool
 * \param Args... the input sequence
 * \return `find_if<...>::type` is `std::integral_constant<std::size_t,v>` where 
`v` is the 0-based index of the first element which satisfy `F`. If no such 
element exists, `v` is `size<Sequence>::value`.
 * \sa tinympl::find_if
 */

/** @} */

/**
 * \defgroup VarModAlgs Modifying sequence operations
 * Algorithms which produce an output sequence
 * @{
 */

/**
 * \class copy
 * \brief Copy the elements from the input sequence to the output sequence
 * \param Out The output sequence type - defaults to the same sequence kind of 
the input sequence
 * \param Args... The input sequence
 * \return `copy<...>::type` is a type templated from `Out` which is constructed 
with the elements of SequenceIn.
 * \sa tinympl::copy
 */
template<template<class ...> class Out, class ... Args> 
struct copy {
    typedef Out<Args...> type;
};

/**
 * \class copy_if
 * \brief Copy the elements of a given input sequence which satisfy a given 
predicate - the ordering is preserved.
 * \param F The test predicate - F<T>::type::value shall be convertible to bool
 * \param Out The output sequence type - defaults to the same sequence kind of 
the input sequence
 * \param Args... The input sequence
 * \return `copy_if<...>::type` is a type templated from `Out` which is 
constructed with the elements of SequenceIn which satisfy the predicate `F`.
 * \sa tinympl::copy_if
 */
template< template<class ... T> class F,
        template<class ...> class Out, 
        class ... Args> struct copy_if;

/**
 * \class copy_n
 * \brief Copy the first n elements from the input sequence.
 * \param n The number of elements to be copied
 * \param Out The output sequence type, defaults to the same kind of the input 
sequence
 * \param Args... the input sequence
 * \return `copy_n<...>::type` is a type templated from `Out` which is 
constructed with the first n types of the input sequence
 * \sa tinympl::copy_n
 */
template<std::size_t n, template<class ...> class Out, class ... Args> 
struct copy_n;

/**
 * \class fill_n
 * \brief Fills an output sequence with n equal elements
 * \param n The number of elements
 * \param T The type of the elements
 * \param Out The output sequence type
 * \return `fill_n<...>::type` is a type templated from `Out` constructed with n 
types equal to `T`
 * \sa tinympl::fill_n
 */
template<std::size_t n, class T, template<class ...> class Out> class fill_n;

/**
 * \class transform
 * \brief Transform an input sequence using a transform function
 * \param F The transform function. `F<T>::type` must be a valid expression
 * \param Out The output sequence type, defaults to the same kind of the input 
sequence
 * \param Args... the input sequence
 * \return `transform<...>::type` is a type templated from `Out` which contains 
the transformed types
 * \sa tinympl::transform
 */
template<template<class ... T> class F, 
        template<class ... > class Out, 
        class ... Args> struct transform;

/**
 * \class generate_n
 * \brief Generate n elements using a given generator metafunction
 * \param n The number of elements to generate
 * \param Gen The generator. `Gen< std::integral_constant<int,i> >::type` must 
be a valid expression.
 * \param Out the output sequence type
 * \return `generate_n<...>::type` is a type templated from `Out` constructed 
with n elements generated with `Gen< int_<0> >, Gen< int_<1> >, ... Gen< 
int_<n-1> >`
 * \sa tinympl::generate_n
 */
template<std::size_t n,
    template<class ...> class Gen,
    template<class ...> class Out> class generate_n;

template<template<class ... T> class F,
        template<class ...> class Out,
        class ... Args> 
struct remove_if : 
    copy_if< bind<logical_not, bind<F, arg1> >::template eval, Out, Args...> {};

/**
 * \class remove
 * \brief Remove all the elements equal to T from the input sequence
 * \param T The element to be removed
 * \param Out The type of the output sequence - defaults to the same kind of the 
input sequence
 * \param Args... The input sequence
 * \return `remove<...>::type` is a type templated from `Out` which contains the 
new sequence
 * \note The comparison is done with \ref tinympl::equal_to - it can be 
specialized
 * \sa tinympl::remove
 */
template< typename T, template<class ... > class Out, class ... Args> 
struct remove : 
    remove_if< bind<equal_to, arg1, T>::template eval_t, Out, Args...> {};

/**
 * \class remove_if
 * \brief Remove the elements from the input sequence if they satisfy a given 
predicate
 * \param F The predicate, `F<T>::type::value` must be convertible to bool
 * \param Out The output sequence type, defaults to the same kind of the input 
sequence
 * \param Args... The input sequence
 * \return `remove_if<...>::type` is a type templated from `Out` which contains 
the new sequence
 * \sa tinympl::remove
 */

template<template<class ... T> class F,
        class T,
        template<class ...> class Out, 
        class ... Args> struct replace_if;

/**
 * \class replace
 * \brief Replace all the elements in the input sequence equal to *Old* with 
*New*
 * \param Old The type to be replaced
 * \param New The new type
 * \param Out The type of the output sequence, defaults to the same kind of the 
input sequence
 * \param Args... The input sequence
 * \return `replace<...>::type` is a type templated from `Out`
 * \note The comparison is done with \ref tinympl::equal_to - it can be 
specialized
 * \sa tinympl::replace
 */
template<class Old,
        class New,
        template<class...> class Out,
        class ... Args> 
struct replace : 
    replace_if<bind<equal_to, arg1, Old>::template eval_t, New, Out, Args...> 
{};

/**
 * \class replace_if
 * \brief Replace all the elements in the input sequence which satisfy a given 
predicate with a given type T
 * \param F The predicate, `F<T>::type::value` must be convertible to bool
 * \param T The type used to replace the types
 * \param Out The type of the output sequence, defaults to the same kind of the 
input sequence
 * \param Args... The input sequence
 * \return `replace_if<...>::type` is a type templated from `Out`
 * \sa tinympl::replace_if
 */

/**
 * \class reverse
 * \brief Reverse the input sequence
 * \param Out the output sequence type, defaults to the same kind of the input 
sequence type
 * \param Args... the input sequence
 * \return `reverse<...>::type` is a type templated from `Out` which contains 
the reversed sequence
 * \sa tinympl::reverse
 */
template<template<class ...> class Out, class ... Args> struct reverse;

/**
 * \class unique
 * \brief Produces a sequence of unique elements from the input sequence, 
preserving the ordering.
 * \param Out the output sequence type - defaults to the same kind of the input 
sequence.
 * \param Args... The input sequence.
 * \return `unique<...>::type` is a type templated from `Out` which contains the 
resulting sequence.
 * \note Only the first (leftmost) duplicate is mantained in the output 
sequence.
 * \sa tinympl::unique
 */
template<template<class ...> class Out, class ... Args> struct unique;

/** @} */

/**
 * \defgroup VarMaxMin Minimum/maximum operations
 * Algorithms which compute the minimum/maximum of a sequence
 * @{
 */

/**
 * \class min_element
 * \brief Compute the index of the smallest element in a sequence
 * \param Cmp the comparator function; `Cmp<A,B>::type::value` must be 
convertible to bool. Defaults to \ref tinympl::less
 * \param Args... the input sequence
 * \return `min_element<...>::type` is an 
`std::integral_constant<std::size_t,v>` where `v` is the 0-based index of the 
minimum element
 * \sa tinympl::min_element
 */
template<template<class ... > class Cmp, class ... Args> struct min_element;

/**
 * \class max_element
 * \brief Compute the index of the largest element in a sequence
 * \param Cmp the comparator function; `Cmp<A,B>::type::value` must be 
convertible to bool. Defaults to \ref tinympl::less
 * \param Args... the input sequence
 * \return `max_element<...>::type` is an 
`std::integral_constant<std::size_t,v>` where `v` is the 0-based index of the 
maximum element
 * \sa tinympl::max_element
 */
template<template<class ... > class Cmp, class ... Args> 
struct max_element : 
    min_element< bind<Cmp, arg2, arg1 >::template eval_t, Args... > {};

/** @} */

/**
 * \defgroup VarSort Sorting operations
 * Algorithms to sort a sequence.
 * @{
 */

/**
 * \class sort
 * \brief Sort the input sequence according to a given comparison function
 * \param Out the output sequence type, defaults to the same kind of the input 
sequence type
 * \param Cmpl The comparison operator. `Cmp<A,B>::type::value` must be 
convertible to bool. The comparator must produce total ordering between 
elements. Defaults to \ref tinympl::less
 * \param Args... the input sequence
 * \note The compile time complexity is O(N^2)
 * \return `sort<...>::type` is a type templated from `Out` which contains the 
sorted sequence
 * \sa tinympl::sort
 */
template<template<class ... > class Cmp,
        template<class ...> class Out,
        class ... Args> class sort;

/** @} */

/**
 * \defgroup VarSet Set operations (on unsorted sequences)
 * Algorithms which perform set operations.
 * \note Unlike the `std` counterparts, these algorithms do not require an 
ordering of the elements.
 * @{
 */

/**
 * \class is_unique
 * \brief Determines whether the input sequence contains only unique elements
 * \param Args... the input sequence
 * \return `is_unique<...>::type` is a `std::integral_constant<bool,v>` where 
`v` is true iff the input sequence contains no duplicates
 * \note Unlike `std::sort`, the input sequence is not required to be sorted, 
but the compile time complexity is O(N^2)
 * \note The comparison is done with \ref tinympl::equal_to - it can be 
specialized
 * \sa tinympl::is_unique
 */
template<class ... Args> struct is_unique;

/** @} */

/**
 * \defgroup VarFold Folding operations
 * Algorithms which perform reduction operations on a sequence.
 * @{
 */

/**
 * \class left_fold
 * \brief Collapses a sequence starting from left using a functor.
 * \param F The functor; `F<T,U>` must be a valid expression
 * \param Args... The input sequence
 * \return A type which is the result of `F( ... F(F(A1,A2),A3) .. )`
 * \sa tinympl::left_fold
 */
template<template<class ...> class Op, class ... Args> struct left_fold;

/**
 * \brief Alias for left_fold.
 * \see left_fold
 * \sa tinympl::accumulate
 */
template<template<class ...> class Op, class ... Args> 
using accumulate = left_fold<Op, Args...>;

/**
 * \class right_fold
 * \brief Collapses a sequence starting from right using a functor.
 * \param F The functor; `F<T,U>` must be a valid expression
 * \param Args... The input sequence
 * \return A type which is the result of `F(A1, F(A2,F(A3, ... ) ) )`
 * \sa tinympl::right_fold
 */
template<template<class ...> class Op, class ... Args> struct right_fold;

/** @} */

/** @} */

/*
 * all_of
 */
template< template<class ... T> class F, class Head, class ... Args> 
struct all_of<F, Head, Args...> :
        std::conditional <
        F<Head>::type::value,
        typename all_of<F, Args...>::type,
        std::integral_constant<bool, false> >::type
{};

template< template<class ... T> class F> struct all_of<F> :
        std::integral_constant<bool, true>
{};

/*
 * any_of
 */
template< template<class ... T> class F, class Head, class ... Args> 
struct any_of<F, Head, Args...> :
        std::conditional <
        F<Head>::type::value,
        std::integral_constant<bool, true>,
        typename any_of<F, Args...>::type >::type
{};

template< template<class ... T> class F> struct any_of<F> :
        std::integral_constant<bool, false>
{};

/*
 * none_of
 */
template< template<class ... T> class F, class Head, class ... Args> 
struct none_of<F, Head, Args...> :
        std::conditional <
        F<Head>::type::value,
        std::integral_constant<bool, false>,
        typename none_of<F, Args...>::type >::type
{};

template< template<class ... T> class F> struct none_of<F> :
        std::integral_constant<bool, true>
{};

/*
 * count_if
 */
template<template<class ... T> class F, class Head, class ... Tail> 
struct count_if<F, Head, Tail...> :
        std::integral_constant < std::size_t,
        count_if<F, Tail...>::type::value +
( F<Head>::type::value ? 1 : 0 ) >
{};

template<template<class ... T> class F> struct count_if<F> :
        std::integral_constant<std::size_t, 0>
{};

/*
 * find_if
 */
template<template<class ...T> class F, class Head, class ... Tail> 
struct find_if<F, Head, Tail...> :
        std::conditional < F<Head>::type::value,
        std::integral_constant<std::size_t, 0>,
        std::integral_constant < std::size_t, 1 +
        find_if<F, Tail...>::type::value >
        >::type
{};

template<template<class ...T> class F> struct find_if<F> :
        std::integral_constant<std::size_t, 0>
{};

/*
 * copy_if
 */
template<template<class ... T> class F, 
        template<class ...> class Out, 
        class Head,
        class ... Tail> 
class copy_if<F, Out, Head, Tail...> {

    template<class ... CopiedElements> 
    struct impl {
        template<class ... Args> 
        using next = typename copy_if<F, Out, Tail...>::template 
            impl<CopiedElements..., Args...>;

        typedef typename std::conditional < F<Head>::type::value,
                                            typename next<Head>::type,
                                            typename next<>::type
                                          >::type type;
    };

    template<template<class ... T> class,
            template<class ...> class,
            class ...> friend class copy_if;

public:
    typedef typename impl<>::type type;
};

template<template<class ... T> class F, template<class ...> class Out> 
class copy_if<F, Out> {
    template<class ... CopiedElements> 
    struct impl {
        typedef Out<CopiedElements...> type;
    };

    template<template<class ... T> class, template<class ...> class, class ...> 
    friend class copy_if;

public:
    typedef typename impl<>::type type;
};

/*
 * copy_n
 */
template<std::size_t n,
        template<class ...> class Out,
        class Head, class ... Tail> 
class copy_n<n, Out, Head, Tail...> {
    
    template<class ... CopiedElements> struct impl {
        typedef typename copy_n < n - 1, Out, Tail... >::template 
            impl<CopiedElements..., Head>::type type;
    };

    template<std::size_t, template<class ...> class, class ...> 
    friend class copy_n;

public:
    static_assert( n <= 1 + sizeof...( Tail ), "n overflow" );
    typedef typename impl<>::type type;
};

template<template<class ...> class Out, class Head, class ... Tail> 
class copy_n<0, Out, Head, Tail...> {
    template<class ... CopiedElements> struct impl {
        typedef Out<CopiedElements...> type;
    };

    template<std::size_t, template<class ...> class, class ...> 
    friend class copy_n;

public:
    typedef typename impl<>::type type;
};

template<template<class ...> class Out> class copy_n<0, Out> {
    template<class ... CopiedElements> struct impl {
        typedef Out<CopiedElements...> type;
    };

    template<std::size_t, template<class ...> class, class ...> 
    friend class copy_n;

public:
    typedef typename impl<>::type type;
};

/*
 * fill_n
 */
template<std::size_t n, class T, template<class ...> class Out> class fill_n {
    template<class ... Args> struct impl {
        typedef typename fill_n < n - 1, T, Out >::template 
            impl<Args..., T>::type type;
    };

    template<std::size_t, class, template<class ...> class> 
    friend class fill_n;

public:
    typedef typename impl<>::type type;
};

template<class T, template<class...> class Out> class fill_n<0, T, Out> {
    template<class ... Args> struct impl {
        typedef Out<Args...> type;
    };

    template<std::size_t, class, template<class ...> class> 
    friend class fill_n;

public:
    typedef typename impl<>::type type;
};

/*
 * transform
 */
template< template<class ... T> class F,
        template<class ... > class Out,
        class ... Args> 
struct transform {
    typedef Out< typename F<Args>::type ... > type;
};

/*
 * generate_n
 */
template< std::size_t n, template<class ...> class Gen, template<class ...> 
class Out> 
class generate_n {
    template<int i, class ... Ts>
    struct impl {
        typedef typename Gen< std::integral_constant<int, i> >::type new_type;
        typedef typename generate_n < n - 1, Gen, Out >::
            template impl < i + 1, Ts..., new_type >::type type;
    };
    
    template<std::size_t, template<class ...> class, template<class ...> class> 
    friend class generate_n;

public:
    typedef typename impl<0>::type type;
};

template< template<class ...> class Gen, template<class ...> class Out>
class generate_n<0, Gen, Out> {
    template<int i, class ... Ts>
    struct impl {
        typedef Out<Ts...> type;
    };

    template<std::size_t, template<class ...> class, template<class ...> class> 
    friend class generate_n;

public:
    typedef typename impl<0>::type type;
};

/*
 * replace_if
 */
template<template<class ... T> class F,
        class T,
        template<class ...> class Out,
        class Head,
        class ... Tail> 
class replace_if<F, T, Out, Head, Tail...> {
    
    template<class ... CopiedElements>
    struct impl {
        typedef typename replace_if<F, T, Out, Tail...>::template 
            impl <CopiedElements...,
                typename std::conditional <
                    F<Head>::type::value,
                    T,
                    Head >::type
                >::type type;
    };

    template<
        template<class ...> class,
        typename,
        template<class...> class, 
        class ...> friend class replace_if;

public:
    typedef typename impl<>::type type;
};

template<template<class ... T> class F, class T, template<class ...> class Out> 
class replace_if<F, T, Out> {
    template<class ... CopiedElements>
    struct impl {
        typedef Out<CopiedElements...> type;
    };

    template<template<class ...> class,
            typename,
            template<class...> class, 
            class ...> friend class replace_if;

public:
    typedef typename impl<>::type type;
};

/*
 * reverse
 */
template<template<class ...> class Out, class Head, class ... Tail>
class reverse<Out, Head, Tail...> {
    template<class ... ReversedTail>
    struct impl {
        typedef typename reverse<Out, Tail...>::template 
            impl<Head, ReversedTail...>::type type;
    };

    template<template<class ...> class, class ...> friend class reverse;

public:
    typedef typename impl<>::type type;
};

template<template<class ...> class Out> class reverse<Out> {
    template<class ... ReversedTail>
    struct impl {
        typedef Out<ReversedTail...> type;
    };

    template<template<class ...> class, class ...> friend class reverse;

public:
    typedef Out<> type;
};

/*
 * unique
 */
template<template<class ...> class Out, class Head, class ... Tail>
class unique<Out, Head, Tail...> {
    template<class ... Ts>
    struct impl {
        template<class ... Us> using next = unique<Out, Us...>;
        typedef typename remove<Head, next, Tail...>::type::
            template impl<Ts..., Head>::type type;
    };

    template<template<class...> class, class...> friend class unique;

public:
    typedef typename impl<>::type type;

};

template<template<class ...> class Out> class unique<Out> {
    template<class ... Ts>
    struct impl {
        typedef Out<Ts...> type;
    };

    template<template<class...> class, class...> friend class unique;

public:
    typedef typename impl<>::type type;
};

namespace detail {

template<template<class ...> class Comp, class ... > class min_element_impl;
template<template<class ...> class Comp, class Head, class ... Tail> class 
min_element_impl<Comp, Head, Tail...> {
    enum {
        next_min = min_element_impl<Comp, Tail...>::type::value
    };

    enum {
        this_min = ! Comp<at_t<next_min, Tail...>, Head>::type::value
    };

public:
    typedef std::integral_constant < std::size_t,
            ( this_min ?
              0 :
              next_min + 1 ) > type;
};

template<template<class ... > class Comp, class Head> struct 
min_element_impl<Comp, Head> {
    typedef std::integral_constant<std::size_t, 0> type;
};

}

/*
 * min element
 */
template<template<class ...> class Comp, class ... Args> 
struct min_element :
        detail::min_element_impl<Comp, Args...>::type
{};

/*
 * sort
 */
template<template<class ... > class Comp,
        template<class ...> class Out,
        class ... Args> 
class sort {
    template<class ... OtherArgs>
    using next_sort = sort<Comp, Out, OtherArgs...>;

    enum {this_min = min_element<Comp, Args...>::type::value };
    typedef typename erase < this_min, this_min + 1, next_sort, Args ... >::type 
        next;

    template<class ... CopiedElements>
    struct impl {
        typedef typename next::template impl<CopiedElements...,
                                        typename at<this_min, Args...>::type 
                                        >::type type;
    };

    template<template<class ... > class , template<class ...> class, class ...> 
    friend class sort;

public:
    typedef typename impl<>::type type;
};

template<template<class ... > class Comp, template<class ...> class Out> 
class sort<Comp, Out> {
    template<class ... CopiedElements>
    struct impl {
        typedef Out<CopiedElements...> type;
    };

    template<template<class ... > class , template<class ...> class, class ...> 
    friend class sort;

public:
    typedef typename impl<>::type type;
};


/*
 * is_unique
 */
template<class Head, class ... Tail> 
struct is_unique<Head, Tail...> : std::conditional <
    find<Head, Tail...>::type::value == sizeof ...( Tail ),
    typename is_unique<Tail...>::type,
    std::integral_constant<bool, false> >::type
{};

template<> struct is_unique<> :
        std::integral_constant<bool, true>
{};

/*
 * left_fold
 */
template<template<class ...> class Op, class Head, class Next, class ... Tail> 
struct left_fold<Op, Head, Next, Tail...> {
    typedef typename left_fold <
        Op,
        typename Op<Head, Next>::type,
        Tail... >::type type;
};

template<template<class ...> class Op, typename T> 
struct left_fold<Op, T> {
    typedef T type;
};

/*
 * right_fold
 */
template<template<class ...> class Op, class Head, class ... Tail> 
struct right_fold<Op, Head, Tail...> {
    typedef typename Op < Head,
            typename right_fold<Op, Tail...>::type >::type type;
};

template<template<class ...> class Op, typename T> struct right_fold<Op, T> {
    typedef T type;
};

}
}

#endif // TINYMPL_ALGORITHM_VARIADIC_HPP 
