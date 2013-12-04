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

#ifndef TINYMPL_BIND_HPP
#define TINYMPL_BIND_HPP

#include "variadic.hpp"

namespace tinympl {


template< template<class ... T> class F,class ... Args> struct bind;

template<std::size_t> struct arg;
typedef arg<1> arg1;
typedef arg<2> arg2;
typedef arg<3> arg3;
typedef arg<4> arg4;
typedef arg<5> arg5;
typedef arg<6> arg6;
typedef arg<7> arg7;
typedef arg<8> arg8;

template<class T> struct is_placeholder : std::false_type {};
template<std::size_t i> struct is_placeholder< arg<i> > : std::integral_constant<std::size_t,i> {};

template<class T> struct is_bind_expression : std::false_type {};
template<template<class ... T> class F,class ... Args> struct is_bind_expression< bind<F,Args...> > : std::true_type {};

template< template<class ... T> class F,class Head,class ... Tail> class bind<F,Head,Tail...>
{	
	template<class ... Args>
	struct call
	{
		template<class ... BoundArgs>
		struct eval
		{
			template<class T,class Enable = void> struct pick {typedef T type;};
			template<class T> struct pick<T, typename std::enable_if< is_placeholder<T>::type::value>::type> {typedef variadic::at_t<is_placeholder<T>::value-1, Args ... > type;};
			template<class T> struct pick<T, typename std::enable_if< is_bind_expression<T>::type::value>::type> {typedef typename T::template eval<Args...>::type type;};
			
			typedef typename pick<Head>::type argument_t;
			
			//Forward the call to bind
			typedef typename bind<F,Tail...>::template call<Args...>::template eval<BoundArgs..., argument_t>::type type;
		};
	};
	
	template< template<class ...> class,class ...> friend class bind;

public:
	template<class ... Args>
	struct eval
	{
		using type = typename call<Args...>::template eval<>::type;
	};
	
	template<class ... Args>
	using eval_t = typename eval<Args...>::type;
};

template< template<class ... T> class F> class bind<F>
{
	template<class ... Args>
	struct call
	{
		template<class ... BoundArgs>
		struct eval
		{
			typedef typename F<BoundArgs...>::type type;
		};
	};
	
	template< template<class ...> class,class ...> friend class bind;

public:
	template<class ... Args>
	struct eval
	{
		using type = typename call<Args...>::template eval<>::type;
	};
	
	template<class ... Args>
	using eval_t = typename eval<Args...>::type;
};

}

#endif // MPL_BIND_HPP 
