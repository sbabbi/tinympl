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

#ifndef TINYMPL_STRING_HPP
#define TINYMPL_STRING_HPP

#include "algorithm.hpp"
#include "sequence.hpp"

namespace tinympl
{

template<class T,T ... chars>
struct basic_string
{	
	enum {size = sizeof ...( chars )};
	enum {emtpy = ( size == 0 )};
	
	typedef basic_string<T,chars...> type;
	typedef T value_type;
	typedef const T * const_pointer;
	
	constexpr static value_type at(std::size_t i) {return c_str()[i];}
	constexpr static const_pointer c_str() {return v_;}
	constexpr static value_type front() {return at(0);}
	constexpr static value_type back() {return at(size - 1);}
	
private:
	template<T t>
	using wrap = std::integral_constant<T,t>;
	
	template<class ... Us>
	using unwrap = basic_string<T, Us::value ... >;

public:

	template<std::size_t pos,class Str>
	using insert = tinympl::insert<pos, 
			Str,
			basic_string<T,chars...> >;
	
	template<std::size_t pos,T ... NewChars>
	using insert_c = insert<pos, basic_string<T,NewChars...> >;
	
	template<std::size_t pos,std::size_t count>
	using erase = tinympl::erase<pos,pos + count,
		basic_string<T,chars...> >;
		
	template<class Str>
	using append = insert<size,Str>;
	
	template<T ... NewChars>
	using append_c = insert_c<size,NewChars...>;
	
	template<std::size_t pos, std::size_t count>
	class substr
	{
		static_assert( pos <= size, "substr pos out of range ");
		static_assert( pos + count <= size, "substr count out of range");

	public:
		typedef typename tinympl::erase<0,pos,
			typename tinympl::erase<pos+count,size, basic_string >::type>::type type;
	};
	
	template<std::size_t pos, std::size_t count, class Str>
	class replace
	{
		static_assert( pos <= size, "substr pos out of range ");
		static_assert( pos + count <= size, "substr count out of range");
		
		typedef typename erase<pos,count>::type str_cleared;
		
	public:
		typedef typename str_cleared::template insert<pos,Str>::type type;
	};
	
	template<std::size_t pos, std::size_t count, T ... ts>
	using replace_c = replace<pos,count, basic_string<T,ts...> >;

	template<class OtherStr>
	using compare = lexicographical_compare<
		basic_string<T,chars...>,
		OtherStr>;

private:
	template<std::size_t i,std::size_t count>
	using unguarded_substr = substr<i, (i+count <= size ? count : size - i)>;
	
	template<class Str,std::size_t i> struct find_impl :
		std::conditional<
			unguarded_substr<i,Str::size>::type::template
				compare<Str>::value == 0,
			std::integral_constant<std::size_t, i>,
			find_impl<Str,i+1> >::type {};
			
	template<class Str> struct find_impl< Str,size > : std::integral_constant< std::size_t, size > {};

	template<class Str,int i> struct rfind_impl :
		std::conditional<
			unguarded_substr<i,Str::size>::type::template
				compare<Str>::value == 0,
			std::integral_constant<std::size_t, i>,
			rfind_impl<Str,i-1> >::type {};
	
	template<class Str> struct find_impl< Str,-1 > : std::integral_constant< std::size_t, size > {};

public:
	template<class Str>
	using find = find_impl<Str,0>;
	
	template<class Str>
	using rfind = rfind_impl<Str,size>;
	
	template<T ... ts> using find_c = find< basic_string<T,ts...> >;
	template<T ... ts> using rfind_c = rfind< basic_string<T,ts...> >;
	
private:
	static constexpr T v_[size + 1] = {chars ... , 0};
};

template<class T,T ... chars>
constexpr T basic_string<T,chars...>::v_ [ size + 1];


template<class T, const T * ptr>
struct make_basic_string
{
	template<T ... ts> struct extract
	{
		typedef typename std::conditional<
			ptr[ sizeof ... (ts) ] == 0,
			basic_string<T, ts...>,
			extract<ts..., ptr[sizeof ... (ts) ]>
		>::type::type type;
	};
	using type = typename extract<>::type;
};

template<const char * p>
using string = typename make_basic_string<char,p>::type;

template<class T,T ... ts> struct as_sequence<basic_string<T,ts...> >
{
	template<class ... Args>
	struct do_rebind
	{
		template<class U>
		using check_t = std::is_same<typename U::value_type,T>;
		
		static_assert(variadic::all_of<check_t,Args...>::value, 
					  "basic_string: unable to rebind when the arguments are not of the same value_type" );
		
		typedef basic_string<T, Args::value ...> type;
	};

	typedef sequence< std::integral_constant<T,ts> ...> type; 
	template<class ... Ts> using rebind = typename do_rebind<Ts...>::type;
};

}

#endif // TINYMPL_STRING_HPP 
