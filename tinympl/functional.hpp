/*
 * tinympl - mini MPL library for C++11
 * Copyright (C) 2013  Ennio Barbaro <e.barbaro@sssup.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TINYMPL_FUNCTIONAL_HPP
#define TINYMPL_FUNCTIONAL_HPP

#include <type_traits>

namespace tinympl {

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

/**
 * \class plus Sums its arguments
 * \return plus<Args...>::type is a std::integral_constant<T,v> where T is the common type between the Args::type... and v is the sum of Args::value ...
 */
template<class ... Args> struct plus;
template<class Head,class ... Tail> struct plus<Head,Tail...>
{
	typedef typename plus<Tail...>::type tail_sum;
	typedef std::integral_constant<
		typename std::common_type<
			typename Head::value_type,
			typename tail_sum::value_type>::type,
		tail_sum::value + Head::value
	> type;
};

template<class Head> struct plus<Head>
{
	typedef Head type;
};

/**
 * \class multiplies Multiplies its arguments
 * \return plus<Args...>::type is a std::integral_constant<T,v> where T is the common type between the Args::type... and v is the product of Args::value ...
 */
template<class ... Args> struct multiplies;
template<class Head,class ... Tail> struct multiplies<Head,Tail...>
{
	typedef typename multiplies<Tail...>::type tail_mult;
	typedef std::integral_constant<
		typename std::common_type<
			typename Head::value_type,
			typename tail_mult::value_type>::type,
		tail_mult::value * Head::value
	> type;
};

template<class Head> struct multiplies<Head>
{
	typedef Head type;
};

/**
 * \class minus Computes A-B where A and B are compile time constants
 * \return minus<A,B>::type is a std::integral_constant<T,v> where T is the common type between A and B, and v is A::value - B::value
 */
template<class A,class B> struct minus
{
	typedef std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type>::type,
		A::value - B::value> type;
};

/**
 * \class divides Computes A/B where A and B are compile time constants
 * \return divides<A,B>::type is a std::integral_constant<T,v> where T is the common type between A and B, and v is A::value / B::value
 */
template<class A,class B> struct divides
{
	typedef std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type>::type,
		A::value / B::value> type;
};

/**
 * \class modulus Computes A%B where A and B are compile time constants
 * \return modulus<A,B>::type is a std::integral_constant<T,v> where T is the common type between A and B, and v is A::value % B::value
 */
template<class A,class B> struct modulus
{
	typedef std::integral_constant<
		typename std::common_type<
			typename A::value_type,
			typename B::value_type>::type,
		A::value % B::value> type;
};

/**
 * \class negate Returns the opposite of the compile time constant A
 * \return negate<A>::type is std::integral_constant<A::value_type,-A::value>
 */
template<class A> struct negate
{
	typedef std::integral_constant<typename A::value_type, - A::value> type;
};

/**
 * \class equal_to Determines whether the types A and B are equal
 * \return equal_to<A,B>::type is a std::integral_constant<bool,v> where v is true iff A and B are equal
 * \note The default behaviour is to forward the call to std::is_same. Users are allowed to specialize this metafunction for user-defined types
 */
template<class A,class B> struct equal_to : std::is_same<A,B> {};

/**
 * \class not_equal_to Determines whether the types A and B are not equal
 * \return not_equal_to<A,B>::type is a std::integral_constant<bool,v> where v is !equal_to<A,B>::value
 */
template<class A,class B> struct not_equal_to {typedef std::integral_constant<bool,! equal_to<A,B>::value > type;};

template<class A,class B> struct greater {typedef std::integral_constant<bool, (A::value > B::value)> type;};
template<class A,class B> struct less {typedef std::integral_constant<bool, (A::value < B::value)> type;};
template<class A,class B> struct greater_equal {typedef std::integral_constant<bool, (A::value >= B::value)> type;};
template<class A,class B> struct less_equal {typedef std::integral_constant<bool, (A::value <= B::value)> type;};

template<class ... Args> struct logical_and;
template<class Head,class ... Tail> struct logical_and<Head,Tail...> {typedef std::integral_constant<bool, Head::value && logical_and<Tail...>::type::value> type;};
template<class Head> struct logical_and<Head> {typedef std::integral_constant<bool,Head::value> type;};

template<class ... Args> struct logical_or;
template<class Head,class ... Tail> struct logical_or<Head,Tail...> {typedef std::integral_constant<bool, Head::value || logical_and<Tail...>::type::value> type;};
template<class Head> struct logical_or<Head> {typedef std::integral_constant<bool,Head::value> type;};

template<class T> struct logical_not {typedef std::integral_constant<bool,!T::value> type;};

/**
 * \class identity Returns the argument passed
 */
template<class T> struct identity {typedef T type;};

/**
 * \class sizeof_ Returns an std::integral_constant<std::size_t,V> where V is the compile time size of the input type
 */
template<class T> struct sizeof_ {typedef std::integral_constant<std::size_t, sizeof(T)> type;};

/**
 * \class inherit Construct a type inherited from the arguments
 */
template<class ... Args> struct inherit
{
	struct inherit_t : Args... {};
	typedef inherit_t type;
};

/**
 * \class if_ Returns A if C::value is true, else B.
 */
template<class C,class A,class B> struct if_ 
{
	typedef typename std::conditional<C::value,A,B>::type type;
};

template<class ... Args> using and_ = logical_and<Args...>;
template<class ... Args> using or_ = logical_or<Args...>;
template<class T> using not_ = logical_not<T>;

}

#endif // TINYMPL_FUNCTIONAL_HPP 
