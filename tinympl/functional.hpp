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

#ifndef TINYMPL_FUNCTIONAL_HPP
#define TINYMPL_FUNCTIONAL_HPP

#include <type_traits>

namespace tinympl {

/**
 * \defgroup NewTypes Type wrappers
 * Templates which wrap a value into a type.
 * @{
 */

//! Compile time integer
template<int i> using int_ = std::integral_constant<int,i>;

//! Compile time short
template<short i> using short_ = std::integral_constant<short,i>;

//! Compile time char
template<char i> using char_ = std::integral_constant<char,i>;

//! Compile time bool
template<bool i> using bool_ = std::integral_constant<bool,i>;

//! Compile time long
template<long i> using long_= std::integral_constant<long,i>;

/** @} */

/**
 * \defgroup Functional Metafunctions
 * Class templates which implement metafunctions
 * @{
 */

/**
 * \defgroup Arithmetic Arithmetic operations
 * Metafunctions which perform arithmetic operations on `std::integral_constant` or equivalent types
 * @{
 */

/**
 * \class plus 
 * \brief Sums its arguments
 * \return `plus<Args...>::type` is a `std::integral_constant<T,v>` where `T` is the common type between the `Args::type...` and `v` is the sum of `Args::value ... `
 */
template<class ... Args> struct plus;
template<class Head,class ... Tail> struct plus<Head,Tail...> : plus<Head, typename plus<Tail...>::type> {};

template<class A,class B> struct plus<A,B> :
	std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type
		>::type, A::value + B::value>
{};

template<class Head> struct plus<Head> : std::integral_constant<typename Head::value_type,Head::value> {};

/**
 * \class multiplies 
 * \brief Multiplies its arguments
 * \return `multiplies<Args...>::type` is a `std::integral_constant<T,v>` where `T` is the common type between the `Args::type...` and `v` is the product of `Args::value ...`
 */
template<class ... Args> struct multiplies;
template<class Head,class ... Tail> struct multiplies<Head,Tail...> : multiplies<Head, typename multiplies<Tail...>::type> {};

template<class A,class B> struct multiplies<A,B> :
	std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type
		>::type, A::value * B::value>
{};

template<class Head> struct multiplies<Head> : std::integral_constant<typename Head::value_type,Head::value> {};

/**
 * \class minus 
 * \brief Computes `A`-`B` where `A` and `B` are compile time type constants
 * \return `minus<A,B>::type` is a `std::integral_constant<T,v>` where `T` is the common type between `A` and `B`, and `v` is `A::value - B::value`
 */
template<class A,class B> struct minus :
	std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type>::type,
		A::value - B::value>
{};

/**
 * \class divides
 * \brief Computes `A`/`B` where `A` and `B` are compile time constants
 * \return `divides<A,B>::type` is a `std::integral_constant<T,v>` where `T` is the common type between `A` and `B`, and `v` is `A::value / B::value`
 */
template<class A,class B> struct divides :
	std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type>::type,
		A::value / B::value>
{};

/**
 * \class modulus
 * \brief Computes `A`%`B` where `A` and `B` are compile time constants
 * \return `modulus<A,B>::type` is a `std::integral_constant<T,v>` where `T` is the common type between `A` and `B`, and `v` is `A::value % B::value`
 */
template<class A,class B> struct modulus :
	std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type>::type,
		A::value % B::value>
{};

/**
 * \class negate 
 * \brief Returns the opposite of the compile time constant A
 * \return `negate<A>::type` is `std::integral_constant<A::value_type,-A::value>`
 */
template<class A> struct negate : std::integral_constant<typename A::value_type, - A::value> {};

/** @} */

/**
 * \defgroup Comparisons Comparisons
 * Metafunctions which perform comparisons operations on `std::integral_constant` or equivalent types
 * @{
 */

/**
 * \class equal_to 
 * \brief Determines whether the types `A` and `B` are equal
 * \return `equal_to<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is true iff `A` and `B` are equal
 * \note The default behaviour is to forward the call to std::is_same. Users are allowed to specialize this metafunction for user-defined types
 */
template<class A,class B> struct equal_to : std::is_same<A,B> {};
template<class T,class U,T t,U u> struct equal_to<
	std::integral_constant<T,t>,
	std::integral_constant<U,u> > : std::integral_constant<bool,t ==u> {};

/**
 * \class not_equal_to 
 * \brief Determines whether the types `A` and `B` are not equal
 * \return `not_equal_to<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is `!equal_to<A,B>::value`
 */
template<class A,class B> struct not_equal_to : std::integral_constant<bool,! equal_to<A,B>::value > {};

/**
 * \class less
 * \brief Determines whether `A` is less than `B`
 * \return `less<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is `A::value < B::value`
 * \note Users are allowed to specialize this metafunction for user-defined types
 * 
 */
template<class A,class B> struct less : std::integral_constant<bool, (A::value < B::value)> {};

/**
 * \class greater 
 * \brief Determines whether `A` is greater than `B`
 * \return `greater<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is `A::value > B::value`
 * \note The default behaviour is to forward the call to \ref less.
 */
template<class A,class B> struct greater : less<B,A> {};

/**
 * \class greater_equal 
 * \brief Determines whether `A` is greater than or equal to `B`
 * \return `greater_equal<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is `A::value >= B::value`
 * \note The default behaviour is to forward the call to \ref less.
 */
template<class A,class B> struct greater_equal : negate< typename less<A,B>::type > {};

/**
 * \class less_equal 
 * \brief Determines whether `A` is less than or equal to `B`
 * \return `less_equal<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is `A::value <= B::value`
 * \note The default behaviour is to forward the call to \ref less.
 */
template<class A,class B> struct less_equal : negate< typename less<B,A>::type > {};

/** @} */

//Convenience logical operations which works directly on bools
template<bool ... Args> struct and_b;
template<bool Head,bool ... Tail> struct and_b<Head,Tail...> : std::integral_constant<bool, Head && and_b<Tail...>::value > {};
template<bool Head> struct and_b<Head> : std::integral_constant<bool,Head> {};

template<bool ... Args> struct or_b;
template<bool Head,bool ... Tail> struct or_b<Head,Tail...> : std::integral_constant<bool, Head || or_b<Tail...>::value> {};
template<bool Head> struct or_b<Head> : std::integral_constant<bool,Head> {};

template<bool v> struct not_b : std::integral_constant<bool, !v> {};

/**
 * \defgroup Logical Logical operations
 * Metafunctions which perform logical operations on `std::integral_constant` or equivalent types
 * @{
 */

/**
 * \class logical_and 
 * \brief Computes the logical and of all its arguments
 */
template<class ... Args> struct logical_and : and_b<Args::value ...> {};
template<class ... Args> using and_ = logical_and<Args...>;

/**
 * \class logical_or 
 * \brief Computes the logical or of all its arguments
 */
template<class ... Args> struct logical_or : or_b<Args::value ...> {};
template<class ... Args> using or_ = logical_or<Args...>;

/**
 * \class logical_not 
 * \brief Negate the argument
 */
template<class T> struct logical_not : not_b<T::value> {};
template<class T> using not_ = logical_not<T>;

/** @} */

/**
 * \class identity 
 * \brief Returns the argument passed
 */
template<class T> struct identity {typedef T type;};

/**
 * \class sizeof_ 
 * \brief Returns an `std::integral_constant<std::size_t,V>` where `V` is the compile time size of the input type
 */
template<class T> struct sizeof_ : std::integral_constant<std::size_t, sizeof(T)> {};

/**
 * \class inherit 
 * \brief Construct a type inherited from the arguments
 */
template<class ... Args> struct inherit
{
	struct inherit_t : Args... {};
	typedef inherit_t type;
};

/**
 * \class if_ 
 * \brief Returns `A` if `C::value` is true, otherwise `B`.
 */
template<class C,class A,class B> struct if_ 
{
	typedef typename std::conditional<C::value,A,B>::type type;
};

/**
 * \class apply
 * \brief Return the result type of the metafunction class F called with arguments Args.,,
 */
template<class F,class ... Args> struct apply
{
	typedef typename F::template eval<Args...>::type type;
};

template<class F,class ... Args>
using apply_t = typename apply<F,Args...>::type;

/** @} */

}

#endif // TINYMPL_FUNCTIONAL_HPP 
