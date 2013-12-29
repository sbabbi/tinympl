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

#ifndef TINYMPL_MAP_HPP
#define TINYMPL_MAP_HPP

#include "algorithm.hpp"

namespace tinympl {

/**
 * \brief Determine whether a type is an `std::pair`
 */
template<class T> struct is_pair : std::false_type {};

template<class FirstType,class SecondType>
struct is_pair<std::pair<FirstType,SecondType> > : std::true_type
{
	typedef FirstType first_type;
	typedef SecondType second_type;
};

/**
 * \ingroup Containers
 * @{
 */

/**
 * \class map
 * \brief A compile time map from a type to another
 * This class represents a compile time mapping between types. The mapping is specified using
 * `std::pair`, that is every parameter of `map` must be an `std::pair<KeyType,ValueType>`.
 * 
 * `map` supports standard insertion/removal and element access.
 * \note \ref equal_to is specialized for `map`, in order to make the order in which the key/value pairs are specified irrelevant.
 */
template<class ... Args> struct map
{
	static_assert( variadic::all_of< is_pair, Args...>::type::value, "All the arguments of a map must be key/value pairs");
	static_assert( variadic::is_unique<typename Args::first_type ...>::type::value,"Duplicate keys in the map");
	
	/**
	 * \class at
	 * \brief Return the value element with the given key
	 */
	template<class T>
	struct at
	{
		enum {index = variadic::find<T, typename Args::first_type ... >::type::value };
		
		static_assert(index < sizeof...(Args),"Key T not present in the map");
		
		typedef typename variadic::at<index,Args...>::type::second_type type;
	};
	
	enum 
	{
		size = sizeof ... (Args) //!< The number of elements contained in the map
	};

	enum 
	{
		empty = (size == 0) //!< Determines whether the map is empty
	};
	
	/**
	 * \brief Count the number of elements in the map with a given key.
	 * \note Since this class is a `map` and not a `multimap`, the only possible results for this operation are 0 and 1.
	 */
	template<class Key>
	using count = std::integral_constant<
			std::size_t,
			(variadic::find<Key, typename Args::first_type ... >::type::value == size ? 0 : 1)>;
	
	/**
	 * \class insert
	 * \brief Returns a new map with the new Key/Value pair, or this map if the key is already present in the map
	 */
	template<class Key,class Value>
	struct insert : std::conditional<
			count<Key>::type::value == 0,
			map<Args..., std::pair<Key,Value> >,
			map<Args...> > {};
	
	/**
	 * \class insert_many
	 * \brief Calls \ref insert many times to insert many Key/Value pairs
	 */
	template<class ... KeyValuePairs>
	struct insert_many
	{
		static_assert( variadic::all_of< is_pair, KeyValuePairs...>::type::value, "All the arguments of insert_many must be key/value pairs");
		template<class Map,class T> using insert_one_t = typename Map::template insert<typename T::first_type, typename T::second_type>;
		
		typedef typename variadic::left_fold<insert_one_t,map,KeyValuePairs...>::type type;
	};
	
	/**
	 * \class erase
	 * \brief Return a new map constructed by the current map removing the `Key` key, if present, otherwise return the current map.
	 */
	template<class Key>
	class erase
	{
		template<class T> using key_comparer = std::is_same<typename T::first_type,Key>;

	public:
		typedef typename variadic::remove_if< key_comparer, map, Args...>::type type;
	};
};

/** @} */

template<class ... As,class ... Bs>
struct equal_to< map<As...>, map<Bs...> > : unordered_equal< sequence<As...>, sequence<Bs...> > {};

}

#endif // TINYMPL_MAP_HPP 
