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

namespace detail
{
//Construct a rational number by forcing a coprime representation
template<std::intmax_t Num, std::intmax_t Den> struct make_rational
{
	typedef std::ratio<Num,Den> ratio_t;
	typedef std::ratio<
		ratio_t::num,
		ratio_t::den> type;
};
}

/**
 * \ingroup NewTypes
 * @{
 */

/**
 * \brief Convenience wrapper around `std::ratio` to automatically reduce `num` and `den` to coprime factors.
 * 
 * `std::is_same< std::ratio<4,2>, std::ratio<2,1> >::value` is `false`, while
 * `std::is_same< rational<4,2>, rational<2,1> >::value` is `true`.
 * 
 * `rational` forwards to `std::ratio`. The comparison functionals \ref plus, \ref minus, \ref multiplies, \ref divides,
 * \ref negate, \ref equal_to and \ref less are specialized to work transparently on `std::ratio`.
 */
template<std::intmax_t Num,std::intmax_t Den> using rational = typename detail::make_rational<Num,Den>::type;

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct plus<
			std::ratio<Num1,Den1>,
			std::ratio<Num2,Den2> > : std::ratio_add<std::ratio<Num1,Den1>, std::ratio<Num2,Den2> > {};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct minus<
			std::ratio<Num1,Den1>,
			std::ratio<Num2,Den2> > : std::ratio_subtract<std::ratio<Num1,Den1>, std::ratio<Num2,Den2> > {};


template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct multiplies<
			std::ratio<Num1,Den1>,
			std::ratio<Num2,Den2> > : std::ratio_multiply<std::ratio<Num1,Den1>, std::ratio<Num2,Den2> > {};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct divides<
			std::ratio<Num1,Den1>,
			std::ratio<Num2,Den2> > : std::ratio_divide<std::ratio<Num1,Den1>, std::ratio<Num2,Den2> > {};

template<std::intmax_t Num,std::intmax_t Den> struct negate<std::ratio<Num,Den> >
{
	typedef std::ratio<-Num,Den> type;
};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct equal_to<
			std::ratio<Num1,Den1>,
			std::ratio<Num2,Den2> > : 
	std::integral_constant<bool, 
		std::ratio<Num1,Den1>::num == std::ratio<Num2,Den2>::num &&
		std::ratio<Num1,Den1>::den == std::ratio<Num2,Den2>::den> {};

template<std::intmax_t Num1,std::intmax_t Den1,
		std::intmax_t Num2,std::intmax_t Den2> struct less<
			std::ratio<Num1,Den1>,
			std::ratio<Num2,Den2> > : std::ratio_less<std::ratio<Num1,Den1>, std::ratio<Num2,Den2> > {};

/** @} */
}

#endif // TINYMPL_RATIO_HPP 
