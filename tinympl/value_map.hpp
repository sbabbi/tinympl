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

#ifndef TINYMPL_VALUE_MAP_HPP
#define TINYMPL_VALUE_MAP_HPP

#include "algorithm.hpp"
#include "map.hpp"

namespace tinympl {

//Map from value to value
template<class KeyType,class ValueType,class ... Args>
struct value_map
{
	template<class T> struct is_valid_key_type : std::false_type {};
	template<KeyType k> struct is_valid_key_type<std::integral_constant<KeyType,k>> : std::true_type {};

	template<class T> struct is_valid_value_type : std::false_type {};
	template<ValueType k> struct is_valid_value_type<std::integral_constant<ValueType,k>> : std::true_type {};

	static_assert( variadic::all_of< is_pair, Args...>::type::value, "All the arguments of a map must be key/value pairs");
	static_assert( variadic::is_unique<typename Args::first_type ...>::type::value,"Duplicate keys in the map");
	static_assert( variadic::all_of< is_valid_key_type, typename Args::first_type...>::type::value, "Wrong type of key");
	static_assert( variadic::all_of< is_valid_value_type, typename Args::second_type...>::type::value, "Wrong type of value");
	
	template<KeyType k> using key = std::integral_constant<KeyType,k>;
	template<ValueType v> using value = std::integral_constant<ValueType,v>;
	
	template<KeyType k,ValueType v>
	using pair = std::pair< key<k>,value<v> >;
	
	template<KeyType k>
	struct at
	{
		enum {index = variadic::find<key<k>, typename Args::first_type ... >::type::value };
		
		static_assert(index < sizeof...(Args),"Key k not present in the map");
		
		typedef typename variadic::at<index,Args...>::type::second_type type;
		enum {value = type::value};
	};
	
	template<class ... Ts>
	using rebind = value_map<KeyType,ValueType,Ts...>;
	
	enum {size = sizeof ... (Args)};
	enum {empty = (size == 0)};
	
	template<KeyType k>
	using count = std::integral_constant<
			std::size_t,
			(variadic::find<key<k>, typename Args::first_type ... >::type::value == size ? 0 : 1)>;
	
	template<KeyType k,ValueType v>
	struct insert
	{
		typedef typename std::conditional<
			count<k>::type::value == 0,
			value_map<KeyType,ValueType,Args..., pair<k,v> >,
			value_map<KeyType,ValueType,Args...> >::type type;
	};
	
	template<class ... KeyValuePairs>
	struct insert_many
	{
		static_assert( variadic::all_of< is_pair, KeyValuePairs...>::type::value, "All the arguments of insert_many must be key/value pairs");
		static_assert( variadic::all_of< is_valid_key_type, typename Args::first_type...>::type::value, "Wrong type of key");
		static_assert( variadic::all_of< is_valid_value_type, typename Args::second_type...>::type::value, "Wrong type of value");

		template<class Map,class T> using insert_one_t = typename Map::template insert<T::first_type::value, T::second_type::value>;
		
		typedef typename variadic::accumulate<value_map,insert_one_t,KeyValuePairs...>::type type;
	};
	
	template<KeyType k>
	class erase
	{
		template<class T> struct key_comparer : std::is_same<typename T::first_type,key<k> > {};

	public:
		typedef typename variadic::remove_if< key_comparer, rebind, Args...>::type type;
	};
};

template<class KeyType,class ValueType,class ... As,class ... Bs>
struct equal_to< value_map<KeyType,ValueType,As...>, value_map<KeyType,ValueType,Bs...> > : unordered_equal< sequence<As...>, sequence<Bs...> > {};

}

#endif // TINYMPL_VALUE_MAP_HPP 
