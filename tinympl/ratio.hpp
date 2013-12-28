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

#ifndef TINYMPL_RATIO_HPP
#define TINYMPL_RATIO_HPP

#include <ratio>
#include "functional.hpp"

namespace tinympl 
{

//! Compile time rational number
template<std::intmax_t Num,std::intmax_t Den> using rational = std::ratio<Num,Den>;

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct plus<
			rational<Num1,Den1>,
			rational<Num2,Den2> > : std::ratio_add<rational<Num1,Den1>, rational<Num2,Den2> > {};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct minus<
			rational<Num1,Den1>,
			rational<Num2,Den2> > : std::ratio_subtract<rational<Num1,Den1>, rational<Num2,Den2> > {};


template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct multiplies<
			rational<Num1,Den1>,
			rational<Num2,Den2> > : std::ratio_multiply<rational<Num1,Den1>, rational<Num2,Den2> > {};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct divides<
			rational<Num1,Den1>,
			rational<Num2,Den2> > : std::ratio_divide<rational<Num1,Den1>, rational<Num2,Den2> > {};

template<std::intmax_t Num,std::intmax_t Den> struct negate<rational<Num,Den> >
{
	typedef rational<-Num,Den> type;
};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct equal_to<
			rational<Num1,Den1>,
			rational<Num2,Den2> > : 
	std::integral_constant<bool, 
		rational<Num1,Den1>::num == rational<Num2,Den2>::num &&
		rational<Num1,Den1>::den == rational<Num2,Den2>::den> {};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct less<
			rational<Num1,Den1>,
			rational<Num2,Den2> > : std::ratio_less<rational<Num1,Den1>, rational<Num2,Den2> > {};

}

#endif // TINYMPL_RATIO_HPP 
