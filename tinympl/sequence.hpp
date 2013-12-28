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

#ifndef TINYMPL_SEQUENCE_HPP
#define TINYMPL_SEQUENCE_HPP

#include <tuple>
#include "variadic.hpp"

namespace tinympl {

template<class ... Args> struct sequence;

template<class T> struct as_sequence;

template<class ... Args> struct as_sequence< std::tuple<Args...> > {
	
	typedef sequence<Args...> type; 
	template<class ... Ts> using rebind = std::tuple<Ts...>;
};

template<class ... Args> struct as_sequence< sequence<Args...> > { 
	
	typedef sequence<Args...> type; 
	template<class ... Ts> using rebind = sequence<Ts...>;
};

template<class T> using as_sequence_t = typename as_sequence<T>::type;

//Determine if a given type is a sequence
template<class T,class = void> struct is_sequence : std::false_type {};
template<class T> struct is_sequence<T,
	typename std::conditional<true, void, as_sequence_t<T> >::type> : std::true_type {};

}

#endif // TINYMPL_SEQUENCE_HPP 
