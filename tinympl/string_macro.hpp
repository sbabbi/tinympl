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

#ifndef TINYMPL_STRING_MACRO_HPP
#define TINYMPL_STRING_MACRO_HPP

#include "string.hpp"

/**
 * \file string_macro.hpp Definition of macros to simplify the creation of a tinympl::string
 */

namespace tinympl
{

template<class T>
using make_mpl_string = typename make_basic_string<typename T::type,T::value>::type;

}

#define TINYMPL_STRING_JOIN2(arg1,arg2) TINYMPL_DO_STRING_JOIN2(arg1,arg2)
#define TINYMPL_DO_STRING_JOIN2(arg1,arg2) arg1 ## arg2

#define MAKE_TINYMPL_STRING(name,str) \
	struct TINYMPL_STRING_JOIN2(tinympl_string_temporary_, name) { \
		typedef char type; \
		static constexpr const char value[] = str; \
	}; \
	typedef tinympl::make_mpl_string<TINYMPL_STRING_JOIN2(tinympl_string_temporary_, name)> name

#endif // TINYMPL_STRING_MACRO_HPP 
