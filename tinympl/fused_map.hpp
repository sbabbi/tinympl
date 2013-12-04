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

#ifndef TINYMPL_FUSED_MAP_HPP
#define TINYMPL_FUSED_MAP_HPP

#include <algorithm>
#include "map.hpp"

namespace tinympl {

template<class ... KeyValuePairs>
struct fused_map : std::tuple<typename KeyValuePairs::second_type ... >
{
	static_assert( variadic::all_of< is_pair, KeyValuePairs...>::type::value, "All the arguments of a map must be key/value pairs");
	static_assert( variadic::is_unique<typename KeyValuePairs::first_type ...>::type::value,"Duplicate keys in the map");

	typedef std::tuple<typename KeyValuePairs::second_type ... > base_type;
	typedef map<KeyValuePairs...> map_type;
	
	using base_type::base_type;
	
	template<class Key>
	typename map_type::template at<Key>::type & at()
	{
		return std::get< map_type::template at<Key>::index >(*this);
	}
	
	template<class Key>
	typename map_type::template at<Key>::type const & at() const
	{
		return std::get< map_type::template at<Key>::index >(*this);
	}
		
	enum {size = map_type::size};
	enum {empty = map_type::empty};

	template<class Key>
	using count = typename map_type::template count<Key>;
};

}

#endif // TINYMPL_FUSED_MAP_HPP 
