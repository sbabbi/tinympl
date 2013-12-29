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

#ifndef TINYMPL_VECTOR_HPP
#define TINYMPL_VECTOR_HPP

#include "algorithm.hpp"
#include "sequence.hpp"

namespace tinympl {

/**
 * \defgroup Containers Containers
 * Full and half compile time containers of types and values.
 * @{
 */
	
/**
 * \class vector
 * \brief A compile time vector of types
 * Vector is the simplest tinympl sequence type. It provides standard modifiers and random access
 * to its elements.
 */
template<class ... Args>
struct vector
{
	enum 
	{
		size = sizeof ... (Args) //!< The size of the vector
	};
	
	enum 
	{
		emtpy = (size == 0) //!< Determine whether the vector is empty
	};
	
	//! Access the i-th element
	template<std::size_t i>
	struct at
	{
		static_assert(i < size,"Index i is out of range");
		typedef typename variadic::at<i,Args...>::type type;
	};
	
	//! Return a new vector constructed by inserting `T` on the back of the current vector
	template<class T>
	struct push_back
	{
		typedef vector<Args...,T> type;
	};
	
	//! Return a new vector constructed by inserting `T` on the front of the current vector
	template<class T>
	struct push_front
	{
		typedef vector<T,Args...> type;
	};
	
	//! Return a new vector constructed by removing the last element of the current vector
	struct pop_back
	{
		typedef typename variadic::erase<size-1,size,vector,Args...>::type type;
	};
	
	//! Return a new vector constructed by removing the first element of the current vector
	struct pop_front
	{
		typedef typename variadic::erase<0,1,vector,Args...>::type type;
	};
	
	//! Return a new vector constructed by erasing the elements in the range [first,last) of the current vector
	template<std::size_t first,std::size_t last>
	struct erase : tinympl::erase<first,last,vector<Args...>,vector> {};

	//! Return a new vector constructed by inserting the elements `Ts...` in the current vector starting at the index `i`
	template<std::size_t i,class ... Ts>
	struct insert : tinympl::insert<i,
		sequence<Ts...>,
		vector<Args...>,
		vector> {};
	
	//! Return the first element of the vector
	struct front
	{
		typedef typename variadic::at<0,Args...>::type type;
	};
	
	//! Return the last element of the vector
	struct back
	{
		typedef typename variadic::at<size-1,Args...>::type type;
	};
};

/** @} */

/**
 * \ingroup SeqCustom
 * \brief Customization point to allow `vector` to work as a tinympl sequence
 */
template<class ... Args> struct as_sequence<vector<Args...> >
{
	typedef sequence<Args...> type; 
	template<class ... Ts> using rebind = vector<Ts...>;
};

}

#endif // TINYMPL_VECTOR_HPP 
