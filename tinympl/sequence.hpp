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

/**
 * \defgroup SeqSupport Sequence support
 * Basic sequences support - provides user defined customization points to define sequences.
 * @{
 */

/**
 * \class sequence
 * \brief The main sequence type.
 */
template<class ... Args> struct sequence;

/**
 * \class as_sequence
 * \brief Provide a customization points by allowing the user to specialize this class
 */
template<class T> struct as_sequence;

/**
 * \defgroup SeqCustom Sequence customization points
 * Allows various sequence types to be used in the sequence algorithms.
 * @{
 */

/**
 * \brief Customization point to allow any variadic template type to work with tinympl
 */
template<class ... Args,template<class ...> class Seq> struct as_sequence< Seq<Args...> > {
	typedef sequence<Args...> type; 
	template<class ... Ts> using rebind = Seq<Ts...>;
};

/** @} */

/**
 * \brief Convenience using declaration to convert a given sequence to a `tinympl::sequence` with the same content
 */
template<class T> using as_sequence_t = typename as_sequence<T>::type;

/**
 * \class is_sequence
 * \brief Metafunction to determine if a given type is a sequence
 */
template<class T,class = void> struct is_sequence : std::false_type {};
template<class T> struct is_sequence<T,
	typename std::conditional<true, void, as_sequence_t<T> >::type> : std::true_type {};

/** @} */
}

#endif // TINYMPL_SEQUENCE_HPP 
