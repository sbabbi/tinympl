tinympl
=======

Ligth version of boost::mpl for C++11.

=======

1) Sequences

This package is built on the concept of sequence of *types*, in short sequence. A sequence is basically a variadic templated class.
The class `sequence` declared in the header <sequence.hpp> is the base class for sequences, every sequence-based algorithm in this package
use this class. The metafunction `as_sequence<T>` allows to specify user-defined types as sequences, in order to make them work transparently
with the algorithms provided.
The user shall specialize `as_sequence<T>` for user-defined sequences types.
`as_sequence<T>` must define:
 
* A member type `type` which *must* be `sequence<Args...>`,  where `Args` are the types contained in the sequence.

* A member variadic template `template<class ...> using rebind = ... `,  such that `rebind<Ts...>` is a sequence of the same type of the original one which contains the types `Ts...`.

The header <sequence.hpp> provides a convenience specialization of `as_sequence<T>` for `std::tuple`.

=======

2) Algorithms

This library provides some compile-time algorithms which mimics the <algorithm> header from the std.

2.1) Variadic algorithms

The header <algorithm_variadic.hpp> contains algorithm which are build around the concept of variadic template parameters.

Example: 

	copy<std::tuple /* The output template type */,  int, char, std::string>::type
is 

	std::tuple<int, char, std::string>

The sequence on which the algorithm operates is expressed via variadic template arguments, and hence it must be the last argument of the algorithm.

Functor object are expressed via variadic template template parameters, like:

	template<class ...> class F

Like boost::mpl they are usually required to define the member `type` which represents the result of the invocation with the given template types.
Usually `type` must be a `std::integral_constant<T, t>`.

2.2) Algorithm based on sequences

The header <algorithm.hpp> contains some algorithm built around the concept of sequence.

Notice that the order of the arguments is different from the variadic algorithm case, and it is much closer to the algorithms from the std.

Example:

	copy< std::tuple<int, char, long> /* Input sequence */, 
		std::tuple /* Output template type */>::type
is

	std::tuple<int, char, long>
	
2.3) List of provided algorithms

These algorithm are available both in the variadic arguments version and in the sequence version.

 * count
 * count_if
 * find
 * find_if
 * copy
 * copy_if
 * copy_n
 * fill_n
 * generate_n
 * transform
 * replace
 * replace_if
 * remove
 * remove_if
 * min_element
 * max_element
 * sort
 * reverse
 * unique
 * is_unique
 * any_of
 * all_of
 * none_of

These algorithm are available only in the sequence version

 * unordered_equal
 * set_union
 * set_difference
 * set_intersection

=======

3) Functionals

Every meta-function object from the standard header <type_traits> (like `std::is_same`, `std::is_base_of`,  etc.) is ready to be used with the algorithms provided in this package.
In addition, the header <functional.hpp> provides some convenience compile-time metafunctions.
Furthermore, this header defines the convenience aliases for `std::integral_constant<T, t>`, so that one can conveniently write
`int_<8>`, `bool_<false>`, `char_<'c'>` instead of the full qualified `std::integral_constant<...>`.

The meta function provided:

 - Algebric metafunctions (operate only on compile time numerical types).
  * `plus`
  * `minus`
  * `multiplies`
  * `divides`
  * `modulus`
  * `negate`
 
 - Comparison metafunctions
  * `equal_to` (like std::is_same, but can be specialized)
  * `not_equal_to`
  * `greater` (only on compile time numerical types, but can be specialized)
  * `less`
  * `greater_equal`
  * `less_equal`
 
- Logical (only on compile time numerical types)
  * `logical_and` (alias `and_`)
  * `logical_or` (alias `or_`)
  * `logical_not` (alias `not_`)
 
- Other
  * `identity`
  * `sizeof_` (computes sizeof(T))
  * `inherit` (computes a type derived from all the arguments)
  * `if_` - like `std::conditional` but the condition argument is a type instead of a bool.

=======

4) Bind

C++11 template-aliases provides a convenient way to bind metafunctions,  but unfortunately the result of this operation is always a named type. Sometimes this is not convenient,  due to excessive
verbosity. The header <bind.hpp> provides a second order metafunction which binds arguments to a given metafunction, much like `std::bind`.

Example:

	bind<std::is_same, arg1, int>::template eval
is equivalent to

	template<class T> using *unspecified* = std::is_same<T, int>;

In addition bind supports nested bind expressions, like:

	bind<std::is_same, bind<std::add_const, arg1>, arg2>::template eval

which is equivalent to:

	template<class T, class U> using *unspecified* = std::is_same< typename std::add_const<T>::type, U>;

Eight placeholders `arg1`,  `arg2`,  `arg8` are provided.


=======

5) Lambda

The second-order metafunction `lambda<>` allows to write compact meta-programming lambda expressions, using every meta-programming functional available.
`lambda<>` works by recursively replacing placeholders and bind expressions in the template definitions of the arguments passed.

Example

	lambda< 
		if_< is_integral<arg1>, 
			float, 
			double>
		>::template eval
is equivalent to

	template<class T> struct unspecified 
	{ 
		typedef typename std::conditional< is_integral<T>::value,  float,  double>::type type;
	};

`lambda<>` treats *every* templated type as a metafunction, that is it try to invoke `T::type`. Sometimes this behavior is not desired, hence the metafunction
`protect` serves to specify that the inner arguments are *not* metafunctions.

Example:

	lambda< std::vector<arg1> >::template eval
is equivalent to

	template<class T> using unspecified = typename std::vector<T, allocator<T>::type >::type;

which is obviously ill-formed.

Instead 

	lambda< protect< std::vector<arg1> > >::template eval
is equivalent to:

	template<class T> using unspecified = std::vector<T,  allocator<T> >;

Lambda also work transparently with bind, that is a bind expression is automatically introspected, and it is not required to specify `bind<...>::template eval`
when using `bind` inside `lambda`.

Note: `lambda<>` only works on functions whose arguments are *only* types. It does not work on the algorithms provided by this package.

=======
	
6) Map and Value Map

The header <map.hpp> defines a compile time container `map`, which is basically a sequence of pairs of Key and Values. 
`map` maps a **type** Key to a **type** Value.
Every argument to map must be an `std::pair<KeyType, ValueType>`,  and all the key types must be different.

The header <value_map.hpp> defines the container `value_map` which maps a **value** key to a **value** value. Every key and value must have the same type, 
which is specified by the first two arguments. The other arguments must be

	std::pair< std::integral_constant<KeyType, k>,  std::integral_constant<ValueType, v> >
	
Which can also be conveniently expressed by `map::pair<k, v>`.

=======

7) Fused map and Fused Valud Map

The header <fused_map.hpp> defines an half-compile time container `fused_map`,  which maps a **type** Key to a *runtime* **value** v of type Value.
This class is inherited from `std::tuple<ValueTypes...>`,  and hence it can be used also like a random-access container via `std::get<>`.

The header<fused_value_map.hpp> defines an half-compile time container `fused_value_map`,  which maps a *compile time* **value** Key to a *runtime* value.
All the keys and values must have the same type. This class is basically a standard_layout wrapper around a plain array, hence it can be initialized via aggregate initialization.
