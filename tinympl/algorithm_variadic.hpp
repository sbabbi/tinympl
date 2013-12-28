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

#ifndef TINYMPL_ALGORITHM_VARIADIC_HPP
#define TINYMPL_ALGORITHM_VARIADIC_HPP

#include "bind.hpp"
#include "functional.hpp"
#include "variadic.hpp"

namespace tinympl { namespace variadic {

template<template<class ... T> class F,class ... Args> struct count_if;
template<typename T,class ... Args> using count = count_if< bind<equal_to,arg1,T>::template eval_t, Args ... >;

template<template<class ... T> class F,class ... Args> struct find_if;
template<typename T,class ... Args> using find = find_if< bind<equal_to,arg1,T>::template eval_t, Args ... >;

template<template<class ... T> class F,template<class ...> class Out,class ... Args> struct copy_if;

template<template<class ...> class Out,class ... Args> struct copy {typedef Out<Args...> type;};
template<std::size_t n,template<class ...> class Out,class ... Args> struct copy_n;
template<std::size_t n,class T,template<class ...> class Out> class fill_n;
template<std::size_t n,template<class ...> class Gen,template<class ...> class Out> class generate_n;

template<template<class ... T> class F, template<class ... > class Out,class ... Args> struct transform;

template<template<class ... T> class F,class T,template<class ...> class Out,class ... Args> struct replace_if;
template<class Old,class New,template<class...> class Out,class ... Args> using replace = replace_if<bind<equal_to,arg1,Old>::template eval_t,New,Out,Args...>;

template<template<class ... T> class F,template<class ...> class Out,class ... Args> using remove_if = copy_if< bind<logical_not,bind<F,arg1> >::template eval,Out,Args...>;
template< typename T, template<class ... > class Out,class ... Args> using remove = remove_if< bind<equal_to, arg1,T>::template eval_t,Out,Args...>;

template<template<class ... > class Cmp,class ... Args> struct min_element;
template<template<class ... > class Cmp,class ... Args> using max_element = min_element< bind<logical_not,bind<Cmp,arg1,arg2> >::template eval_t,Args... >;

template<template<class ... > class Cmp,template<class ...> class Out,class ... Args> class sort;
template<template<class ...> class Out, class ... Args> struct reverse;

template<class ... Args> struct is_unique;
template<template<class ...> class Out,class ... Args> struct unique;

template<class T,template<class ...> class Op,class ... Args> struct accumulate;

template< template<class ... T> class F,class ... Args> struct all_of;
template< template<class ... T> class F,class ... Args> struct any_of;
template< template<class ... T> class F,class ... Args> struct none_of;


/**
 * count_if
 */
template<template<class ... T> class F,class Head,class ... Tail> struct count_if<F,Head,Tail...> :
	std::integral_constant<std::size_t,
		count_if<F,Tail...>::type::value + 
		(F<Head>::type::value ? 1 : 0)>
{};

template<template<class ... T> class F> struct count_if<F> :
	std::integral_constant<std::size_t,0>
{};

/**
 * find_if
 */
template<template<class ...T> class F,class Head,class ... Tail> struct find_if<F,Head,Tail...> :
	std::conditional< F<Head>::type::value,
		std::integral_constant<std::size_t,0>,
		std::integral_constant<std::size_t,1 + 
			find_if<F,Tail...>::type::value> 
		>::type
{};

template<template<class ...T> class F> struct find_if<F> : 
	std::integral_constant<std::size_t,0>
{};

/**
 * copy_if
 */
template<template<class ... T> class F,template<class ...> class Out,class Head, class ... Tail> class copy_if<F,Out,Head,Tail...>
{
	template<class ... CopiedElements> struct impl
	{
		template<class ... Args> using next = typename copy_if<F,Out,Tail...>::template impl<CopiedElements...,Args...>;
		
		typedef typename std::conditional<
			F<Head>::type::value,
				typename next<Head>::type,
				typename next<>::type
			>::type type;
	};
	
	template<template<class ... T> class,template<class ...> class,class ...> friend class copy_if;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ... T> class F,template<class ...> class Out> class copy_if<F,Out>
{
	template<class ... CopiedElements> struct impl
	{
		typedef Out<CopiedElements...> type;
	};

	template<template<class ... T> class,template<class ...> class,class ...> friend class copy_if;

public:
	typedef typename impl<>::type type;
};

/**
 * copy_n
 */
template<std::size_t n,template<class ...> class Out,class Head,class ... Tail> class copy_n<n,Out,Head,Tail...>
{
	template<class ... CopiedElements> struct impl
	{
		typedef typename copy_n<n-1,Out,Tail...>::template impl<CopiedElements...,Head>::type type;
	};

	template<std::size_t,template<class ...> class,class ...> friend class copy_n;

public:
	static_assert(n <= 1 + sizeof...(Tail),"n overflow");
	typedef typename impl<>::type type;
};

template<template<class ...> class Out,class Head,class ... Tail> class copy_n<0,Out,Head,Tail...>
{
	template<class ... CopiedElements> struct impl
	{
		typedef Out<CopiedElements...> type;
	};
	
	template<std::size_t,template<class ...> class,class ...> friend class copy_n;

public:
	typedef typename impl<>::type type;
};

template<template<class ...> class Out> class copy_n<0,Out>
{
	template<class ... CopiedElements> struct impl
	{
		typedef Out<CopiedElements...> type;
	};
	
	template<std::size_t,template<class ...> class,class ...> friend class copy_n;

public:
	typedef typename impl<>::type type;
};

/**
 * fill_n
 */
template<std::size_t n,class T,template<class ...> class Out> class fill_n
{
	template<class ... Args> struct impl
	{
		typedef typename fill_n<n-1,T,Out>::template impl<Args...,T>::type type;
	};
	
	template<std::size_t,class,template<class ...> class> friend class fill_n;
	
public:
	typedef typename impl<>::type type;
};

template<class T,template<class...> class Out> class fill_n<0,T,Out>
{
	template<class ... Args> struct impl
	{
		typedef Out<Args...> type;
	};

	template<std::size_t,class,template<class ...> class> friend class fill_n;

public:
	typedef typename impl<>::type type;
};

/**
 * generate_n
 */
template< std::size_t n, template<class ...> class Gen,template<class ...> class Out> class generate_n
{
	template<int i,class ... Ts>
	struct impl
	{
		typedef typename Gen< std::integral_constant<int,i> >::type new_type;
		typedef typename generate_n<n-1,Gen,Out>::template impl<i+1,Ts...,new_type>::type type;
	};
	template<std::size_t, template<class ...> class,template<class ...> class> friend class generate_n;
	
public:
	typedef typename impl<0>::type type;

};

template< template<class ...> class Gen,template<class ...> class Out> class generate_n<0,Gen,Out>
{
	template<int i,class ... Ts>
	struct impl
	{
		typedef Out<Ts...> type;
	};
	
	template<std::size_t, template<class ...> class,template<class ...> class> friend class generate_n;
	
public:
	typedef typename impl<0>::type type;
};

/**
 * transform
 */
template< template<class ... T> class F, template<class ... > class Out,class ... Args> struct transform
{
	typedef Out< typename F<Args>::type ... > type;
};

/**
 * replace_if
 */
template<template<class ... T> class F,class T,template<class ...> class Out,class Head,class ... Tail> class replace_if<F,T,Out,Head,Tail...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename replace_if<F,T,Out,Tail...>::template impl<CopiedElements...,
			typename std::conditional<
				F<Head>::type::value,
				T,
				Head>::type>::type type;
	};
	
	template<template<class ...> class,typename,template<class...> class,class ...> friend class replace_if;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ... T> class F,class T,template<class ...> class Out> class replace_if<F,T,Out>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out<CopiedElements...> type;
	};
	
	template<template<class ...> class,typename,template<class...> class,class ...> friend class replace_if;
	
public:
	typedef typename impl<>::type type;
};



/**
 * min element
 */

namespace detail
{

template<template<class ...> class Comp,class ... > class min_element_impl;
template<template<class ...> class Comp,class Head,class ... Tail> class min_element_impl<Comp,Head,Tail...>
{
	enum {
		next_min = min_element_impl<Comp,Tail...>::type::value
	};
	
	enum {
		this_min = Comp<Head,at_t<next_min,Tail...>>::type::value
	};

public:
	typedef std::integral_constant<std::size_t,
		(this_min ?
		0 :
		next_min + 1)> type;
};

template<template<class ... > class Comp,class Head> struct min_element_impl<Comp,Head>
{
	typedef std::integral_constant<std::size_t,0> type;
};

}

template<template<class ...> class Comp,class ... Args> struct min_element : 
	detail::min_element_impl<Comp,Args...>::type
{};

/**
 * sort
 */
template<template<class ... > class Comp,template<class ...> class Out,class ... Args> class sort
{
	template<class ... OtherArgs>
	using next_sort = sort<Comp,Out,OtherArgs...>;

	enum {this_min = min_element<Comp,Args...>::type::value };
	typedef typename erase< this_min, this_min + 1,next_sort,Args ... >::type next;
	
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename next::template impl<CopiedElements..., typename at< this_min, Args...>::type>::type type;
	};

	template<template<class ... > class ,template<class ...> class, class ...> friend class sort;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ... > class Comp,template<class ...> class Out> class sort<Comp,Out>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out<CopiedElements...> type;
	};

	template<template<class ... > class ,template<class ...> class, class ...> friend class sort;
	
public:
	typedef typename impl<>::type type;
};


/**
 * reverse
 */
template<template<class ...> class Out, class Head,class ... Tail> class reverse<Out,Head,Tail...>
{
	template<class ... ReversedTail>
	struct impl
	{
		typedef typename reverse<Out,Tail...>::template impl<Head,ReversedTail...>::type type;
	};
	
	template<template<class ...> class,class ...> friend class reverse;
	
public:
	typedef typename impl<>::type type;
};
	
template<template<class ...> class Out> class reverse<Out>
{
	template<class ... ReversedTail>
	struct impl
	{
		typedef Out<ReversedTail...> type;
	};
	
	template<template<class ...> class,class ...> friend class reverse;

public:
	typedef Out<> type;
};

/***
 * is_unique
 */
template<class Head,class ... Tail> struct is_unique<Head,Tail...> :
	std::conditional<
		find<Head,Tail...>::type::value == sizeof ... (Tail),
		typename is_unique<Tail...>::type,
		std::integral_constant<bool,false> >::type
{};

template<> struct is_unique<> :
	std::integral_constant<bool,true>
{};

/**
 * unique
 */

template<template<class ...> class Out,class Head,class ... Tail> class unique<Out,Head,Tail...>
{
	template<class ... Ts>
	struct impl
	{
		template<class ... Us> using next = unique<Out,Us...>;
		typedef typename remove<Head,next,Tail...>::type::template impl<Ts...,Head>::type type;
	};
	
	template<template<class...> class,class...> friend class unique;

public:
	typedef typename impl<>::type type;

};

template<template<class ...> class Out> class unique<Out>
{
	template<class ... Ts>
	struct impl
	{
		typedef Out<Ts...> type;
	};

	template<template<class...> class,class...> friend class unique;

public:
	typedef typename impl<>::type type;
};

/**
 * accumulate
 */

template<typename T,template<class ...> class Op,class Head,class ... Tail> struct accumulate<T,Op,Head,Tail...>
{
	typedef typename accumulate<
		typename Op<T,Head>::type,
		Op,
		Tail...>::type type;
};

template<typename T,template<class ...> class Op> struct accumulate<T,Op>
{
	typedef T type;
};

/**
 * all_of
 */
template< template<class ... T> class F,class Head,class ... Args> struct all_of<F,Head,Args...> : 
	std::conditional<
		F<Head>::type::value,
		typename all_of<F,Args...>::type,
		std::integral_constant<bool,false> >::type
{};

template< template<class ... T> class F> struct all_of<F> :
	std::integral_constant<bool,true>
{};

/**
 * any_of
 */
template< template<class ... T> class F,class Head,class ... Args> struct any_of<F,Head,Args...> :
	std::conditional<
		F<Head>::type::value,
		std::integral_constant<bool,true>,
		typename any_of<F,Args...>::type >::type
{};

template< template<class ... T> class F> struct any_of<F> :
	std::integral_constant<bool,false> 
{};

/**
 * none_of
 */
template< template<class ... T> class F,class Head,class ... Args> struct none_of<F,Head,Args...> :
	std::conditional<
		F<Head>::type::value,
		std::integral_constant<bool,false>,
		typename none_of<F,Args...>::type >::type
{};

template< template<class ... T> class F> struct none_of<F> :
	std::integral_constant<bool,true>
{};

} }

#endif // TINYMPL_ALGORITHM_VARIADIC_HPP 
