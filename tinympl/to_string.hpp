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

#ifndef TINYMPL_TO_STRING_HPP
#define TINYMPL_TO_STRING_HPP

#include "string.hpp"

namespace tinympl
{
namespace detail
{

//Handle generic numbers
template<class T,T value,T base = 10,class = void> struct to_string_impl
{
	typedef typename to_string_impl<T,value / base,base>::type head;
	typedef typename to_string_impl<T,value % base,base>::type tail;
	typedef typename head::template append<tail>::type type;
};

//Handle negative numbers
template<class T,T value,T base> struct to_string_impl<T,value,base, 
	typename std::enable_if<(value < 0)>::type>
{
	typedef typename to_string_impl<T,-value,base>::type tail;
	typedef typename tail::template insert_c<0,'-'>::type type;
};

//Handle one digit numbers
template<class T,T value,T base> struct to_string_impl<T,value,base, 
	typename std::enable_if<(value >= 0 && value < base)>::type>
{
	static_assert( value >= 0 && value < 16,"Base > 16 not supported");

	typedef basic_string<char,
		(value < 10 ?
			'0' + value :
			'a' + value - 10)> type;
};

}

template<class T,T value> using to_string = detail::to_string_impl<T,value>;
template<class T,T value> using to_string_t = typename to_string<T,value>::type;

template<int value> using to_string_i = detail::to_string_impl<int,value>;
template<int value> using to_string_i_t = typename to_string_i<value>::type;

template<long value> using to_string_l = detail::to_string_impl<long,value>;
template<long value> using to_string_l_t = typename to_string_l<value>::type;

template<unsigned value> using to_string_u = detail::to_string_impl<unsigned,value>;
template<unsigned value> using to_string_u_t = typename to_string_u<value>::type;

template<long long value> using to_string_ll = detail::to_string_impl<long long,value>;
template<long long value> using to_string_ll_t = typename to_string_ll<value>::type;

}

#endif // TINYMPL_TO_STRING_HPP 
