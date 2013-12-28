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

#ifndef TINYMPL_VARIADIC_HPP
#define TINYMPL_VARIADIC_HPP

#include <type_traits>

namespace tinympl { namespace variadic {

/**
 * Basic support for variadic templates
 */
template<class ... Args> struct size;
template<std::size_t i,class ... Args> struct at;
template<std::size_t pos,class T,template<class ... > class Out,class ... Args> struct insert;
template<std::size_t start,std::size_t end,template<class ...> class Out,class ... Args> struct erase;

template<class ... Args> using size_t = typename size<Args...>::type;
template<std::size_t i,class ... Args> using at_t = typename at<i,Args...>::type;
template<std::size_t start,std::size_t end,template<class ...> class Out,class ... Args> using erase_t = typename erase<start,end,Out,Args...>::type;

/**
 * size
 */
template<class ... Args> struct size
{
	typedef std::integral_constant<std::size_t,sizeof...(Args)> type;
};

/**
 * at
 */
template<std::size_t i,class Head,class ... Tail> struct at<i,Head,Tail...>
{
	static_assert(i < sizeof ... (Tail) + 1,"Index out of range");
	typedef typename at<i-1,Tail...>::type type;
};

template<class Head,class ... Tail> struct at<0,Head,Tail...>
{
	typedef Head type;
};

/**
 * insert
 */
template<std::size_t pos,class T,template<class ... > class Out,class Head,class ... Args> class insert<pos,T,Out,Head,Args...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename insert<pos-1,T,Out,Args...>::template impl<CopiedElements...,Head>::type type;
	};
	
	template<std::size_t,class,template<class ... > class,class ...> friend class insert;
	
public:
	static_assert(pos <= sizeof ... (Args) + 1,"pos > sequence size!");
	
	typedef typename impl<>::type type;
};

template<class T,template<class ... > class Out,class ... Args> class insert<0,T,Out,Args...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out< CopiedElements ..., T, Args ... > type;
	};
	
	template<std::size_t,class,template<class ... > class,class ...> friend class insert;
	
public:
	typedef typename impl<>::type type;
};

/**
 * erase
 */
template<std::size_t start,std::size_t end,template<class ...> class Out,class Head,class ... Args> class erase<start,end,Out,Head,Args...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename erase<start-1,end-1,Out,Args...>::template impl<CopiedElements...,Head>::type type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend class erase;
	
public:
	static_assert(start <= end,"Start > end!");

	typedef typename impl<>::type type;
};

template<std::size_t end,template<class ...> class Out,class Head,class ... Args> class erase<0,end,Out,Head,Args...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename erase<0,end-1,Out,Args...>::template impl<CopiedElements...>::type type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend class erase;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ...> class Out,class Head,class ... Args> class erase<0,0,Out,Head,Args...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out<CopiedElements..., Head,Args...> type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend class erase;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ...> class Out> class erase<0,0,Out>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out<CopiedElements...> type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend class erase;
	
public:
	typedef typename impl<>::type type;
};


} }

#endif // TINYMPL_VARIADIC_HPP 
