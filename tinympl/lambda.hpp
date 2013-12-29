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

#ifndef TINYMPL_LAMBDA_HPP
#define TINYMPL_LAMBDA_HPP

#include "bind.hpp"

namespace tinympl {

/**
 * \ingroup BindAndLambda
 * @{
 */


template<class T> struct protect {typedef T type;};

template<class Expr> 
struct lambda
{
	template<class ... Ts>
	struct eval
	{
		template<class T,class Enable = void> struct pick {typedef T type;};
		template<class T> struct pick<T, typename std::enable_if< is_placeholder<T>::type::value>::type> {typedef variadic::at_t<is_placeholder<T>::value-1, Ts ... > type;};
		template<class T> struct pick<T, typename std::enable_if< is_bind_expression<T>::type::value>::type> {typedef typename T::template eval<Ts...>::type type;};
		
		typedef typename pick<Expr>::type type;
	};
	
	template<class ... Ts> using eval_t = typename eval<Ts...>::type;
};

template< template<class ...> class F, class ... Args> struct lambda<F<Args...> >
{
	template<class ... Ts>
	struct eval
	{
		template<class T> using forward_t = typename T::template eval<Ts...>::type;
		
		typedef typename F< forward_t<lambda<Args> > ... >::type type;
	};
	
	template<class ... Ts> using eval_t = typename eval<Ts...>::type;
};

template< template<class ...> class F,class ... Args> struct lambda< protect<F<Args...> > >
{
	template<class ... Ts>
	struct eval
	{
		template<class T> using forward_t = typename T::template eval<Ts...>::type;
		
		typedef F< forward_t< lambda<protect<Args> > > ... > type;
	};
	
	template<class ... Ts> using eval_t = typename eval<Ts...>::type;
};

template<class Expr> struct is_bind_expression<lambda<Expr> > : std::true_type {};

/** @} */

}

#endif // TINYMPL_LAMBDA_HPP 
