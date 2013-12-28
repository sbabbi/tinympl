/*
 * <one line to give the program's name and a brief idea of what it does.>
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

template<class ... Args>
struct vector
{
	enum {size = sizeof ... (Args)};
	enum {emtpy = (size == 0)};
	
	template<std::size_t i>
	struct at
	{
		static_assert(i < size,"Index i is out of range");
		typedef typename variadic::at<i,Args...>::type type;
	};
	
	template<class T>
	struct push_back
	{
		typedef vector<Args...,T> type;
	};
	
	template<class T>
	struct push_front
	{
		typedef vector<T,Args...> type;
	};
	
	struct pop_back
	{
		typedef typename variadic::erase<size-1,size,vector,Args...>::type type;
	};
	
	struct pop_front
	{
		typedef typename variadic::erase<0,1,vector,Args...>::type type;
	};
	
	template<std::size_t first,std::size_t last>
	struct erase : tinympl::erase<first,last,vector<Args...>,vector> {};

	template<std::size_t i,class ... Ts>
	struct insert : tinympl::insert<i,
		sequence<Ts...>,
		vector<Args...>,
		vector> {};
	
	struct front
	{
		typedef typename variadic::at<0,Args...>::type type;
	};
	
	struct back
	{
		typedef typename variadic::at<size-1,Args...>::type type;
	};
};

template<class ... Args> struct as_sequence<vector<Args...> >
{
	typedef sequence<Args...> type; 
	template<class ... Ts> using rebind = vector<Ts...>;
};

}

#endif // TINYMPL_VECTOR_HPP 
