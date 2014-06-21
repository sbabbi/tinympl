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

#ifndef TINYMPL_VARIADIC_HPP
#define TINYMPL_VARIADIC_HPP

#include <type_traits>

namespace tinympl { namespace variadic {

/**
 * \defgroup VariadicBasic Basic variadic support
 * Basic support for variadic template sequences
 * @{
 */

/**
 * \class size 
 * \brief Compute the size of a variadic template
 * \return `size<Args...>::value` is equivalent to `sizeof ... (Args)`
 */
template<class ... Args> struct size;

/**
 * \class at 
 * \brief Extract the i-th element of a variadic template
 * \param i The index to extract
 */
template<std::size_t i,class ... Args> struct at;

/**
 * \class insert 
 * \brief Produce an output sequence from a variadic template inserting a new element at a given position
 * \param pos The position of the new element
 * \param T the new element
 * \param Out the output sequence type
 * \param Args... the input variadic template
 */
template<std::size_t pos,class T,template<class ... > class Out,class ... Args> struct insert;

/**
 * \class erase 
 * \brief Produce an output sequence from a variadic template by removin the elements in the given range
 * \param start The index of the first element to be removed
 * \param end One plus the index of the last element to be removed
 * \param Out The output sequence type
 * \param Args... the input variadic template
 */
template<std::size_t start,std::size_t end,template<class ...> class Out,class ... Args> struct erase;

template<class ... Args> using size_t = typename size<Args...>::type;
template<std::size_t i,class ... Args> using at_t = typename at<i,Args...>::type;
template<std::size_t start,std::size_t end,template<class ...> class Out,class ... Args> using erase_t = typename erase<start,end,Out,Args...>::type;

/** @} */

/*
 * size
 */
template<class ... Args> struct size : std::integral_constant<std::size_t,sizeof...(Args)> {};

/*
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

/*
 * insert
 */
template<std::size_t pos,class T,template<class ... > class Out,class Head,class ... Args> struct insert<pos,T,Out,Head,Args...>
{
private:
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename insert<pos-1,T,Out,Args...>::template impl<CopiedElements...,Head>::type type;
	};
	
	template<std::size_t,class,template<class ... > class,class ...> friend struct insert;
	
public:
	static_assert(pos <= sizeof ... (Args) + 1,"pos > sequence size!");
	
	typedef typename impl<>::type type;
};

template<class T,template<class ... > class Out,class Head,class ... Args> struct insert<0,T,Out,Head,Args...>
{
private:
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out< CopiedElements ..., T, Head,Args ... > type;
	};
	
	template<std::size_t,class,template<class ... > class,class ...> friend struct insert;
	
public:
	typedef typename impl<>::type type;
};

template<class T,template<class ... > class Out> struct insert<0,T,Out>
{
private:
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out< CopiedElements ..., T > type;
	};
	
	template<std::size_t,class,template<class ... > class,class ...> friend struct insert;
	
public:
	typedef typename impl<>::type type;
};

/*
 * erase
 */
template<std::size_t start,std::size_t end,template<class ...> class Out,class Head,class ... Args> struct erase<start,end,Out,Head,Args...>
{
private:
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename erase<start-1,end-1,Out,Args...>::template impl<CopiedElements...,Head>::type type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend struct erase;
	
public:
	static_assert(start <= end,"Start > end!");

	typedef typename impl<>::type type;
};

template<std::size_t end,template<class ...> class Out,class Head,class ... Args> struct erase<0,end,Out,Head,Args...>
{
	template<class ... CopiedElements>
	struct impl
	{
		typedef typename erase<0,end-1,Out,Args...>::template impl<CopiedElements...>::type type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend struct erase;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ...> class Out,class Head,class ... Args> struct erase<0,0,Out,Head,Args...>
{
private:
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out<CopiedElements..., Head,Args...> type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend struct erase;
	
public:
	typedef typename impl<>::type type;
};

template<template<class ...> class Out> struct erase<0,0,Out>
{
private:
	template<class ... CopiedElements>
	struct impl
	{
		typedef Out<CopiedElements...> type;
	};
	
	template<std::size_t,std::size_t,template<class ...> class,class ...> friend struct erase;
	
public:
	typedef typename impl<>::type type;
};


} }

#endif // TINYMPL_VARIADIC_HPP 
