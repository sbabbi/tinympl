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

#ifndef TINYMPL_ALGORITHM_HPP
#define TINYMPL_ALGORITHM_HPP

#include "algorithm_variadic.hpp"
#include "functional.hpp"
#include "sequence.hpp"

namespace tinympl {

/**
 * \defgroup SeqAlgs Sequence algorithms
 * Algorithms which operate on sequences
 * @{
 */

/**
 * \defgroup SeqAlgsIntr Sequence introspection operations
 * Algorithms which perform trivial operations on sequences
 * @{
 */

/**
 * \class join
 * \brief Merge two sequences
 * \param Args The sequences
 * \return A sequence constructed by joining all the passed sequences with the type of the first one
 */
template<class ... Args> struct join;
template<class Head,class Next,class ... Tail> struct join<Head,Next,Tail...>
{
	typedef typename join<
	typename join<Head,Next>::type,
		Tail...>::type type;
};

template<class Head,class Last> class join<Head,Last>
{
	template<class S1,class S2,template<class ...> class Out> struct do_join;
	template<class ... S1,class ... S2,template<class ...> class Out> struct do_join< sequence<S1...>, sequence<S2...>,Out >
	{
		typedef Out<S1..., S2...> type;
	};

public:
	typedef typename do_join< as_sequence_t<Head>, as_sequence_t<Last>, as_sequence<Head>::template rebind>::type type;
};

template<class Head> struct join<Head> { typedef Head type;};

/**
 * \class at 
 * \brief Get the i-th element of a sequence
 * \param i The index of the desired element
 * \param Seq The input sequence
*/
template<std::size_t i,class Sequence> struct at : at <i, as_sequence_t<Sequence> > {};
template<std::size_t i,class ... Args> struct at<i, sequence<Args...> > : variadic::at<i,Args...> {};

/**
 * \class size 
 * \brief Get the number of elements of a sequence
 * \param Seq the sequence
 */
template<class Seq> struct size : size< as_sequence_t<Seq> > {};
template<class ... Args> struct size<sequence<Args...> > : variadic::size<Args...> {};

/** 
 * \class insert 
 * \brief Insert a subsequence into a given sequence at a given position
 * \param pos The insertion position in the main sequence
 * \param SubSeq The subsequence
 * \param Seq The main sequence
 * \param Out The output sequence type
 */
template<std::size_t pos,class SubSeq,class Seq,template<class ...> class Out = as_sequence<Seq>::template rebind> struct insert : insert<pos, as_sequence_t<SubSeq>, as_sequence_t<Seq>,Out> {};
template<std::size_t pos,class ... SubSeqArgs,class ... SeqArgs,template<class...> class Out> class insert<pos, sequence<SubSeqArgs...>, sequence<SeqArgs...>,Out>
{
	template<class ... Us>
	using head_seq = sequence<Us ..., SubSeqArgs ... >;

	typedef typename variadic::erase<pos,sizeof ...(SeqArgs),head_seq,SeqArgs ... >::type head;
	typedef typename variadic::erase<0,pos,sequence,SeqArgs ... >::type tail;

public:
	typedef typename join<Out<>,head,tail>::type type;
};

/**
 * \class erase 
 * \brief Remove a range in a given sequence
 * \param first The first element to be removed
 * \param last The first element which is not removed
 * \param Seq The input sequence
 * \param Out The output sequence type
 */
template<std::size_t first,std::size_t last,class Seq,template<class...> class Out = as_sequence<Seq>::template rebind> struct erase : erase<first,last, as_sequence_t<Seq>, Out> {};
template<std::size_t first,std::size_t last,class ... Args,template<class...> class Out> struct erase<first,last,sequence<Args...>,Out> : 
	variadic::erase<first,last,Out,Args...> {};

/** @} */

/**
 * \defgroup SeqNonModAlgs Non-modifying sequence operations
 * Algorithms which analyze a sequence without producing an output sequence
 * @{
 */

/**
 * \class all_of 
 * \brief Determines whether every element in the sequence satisfies the given predicate
 * \param Sequence the input sequence
 * \param F the predicate, `F<T>::type::value` must be convertible to `bool`
 * \return `all_of<...>::type` is a `std::integral_constant<bool,v>` where `v` is true iff all the elements in the sequence satisfy the predicate `F`
 */
template< class Sequence, template<class ...> class F> struct all_of : all_of<as_sequence_t<Sequence>,F> {};
template< template<class ...> class F,class ... Args> struct all_of<sequence<Args...>,F > : variadic::all_of<F,Args...> {};

/**
 * \class any_of
 * \brief Determines whether any of the elements in the sequence satisfy the given predicate
 * \param Sequence the input sequence
 * \param F the predicate, `F<T>::type::value` must be convertible to `bool`
 * \return `any_of<...>::type` is a `std::integral_constant<bool,v>` where `v` is true iff at least one element in the sequence satisfy the predicate `F`
 */
template< class Sequence, template<class ...> class F> struct any_of : any_of<as_sequence_t<Sequence>,F> {};
template< template<class ...> class F,class ... Args> struct any_of<sequence<Args...>,F > : variadic::any_of<F,Args...> {};

/**
 * \class none_of 
 * \brief Determines whether none of the elements in the sequence satisfy the given predicate
 * \param Sequence the input sequence
 * \param F the predicate, `F<T>::type::value` must be convertible to bool
 * \return `none_of<...>::type` is a `std::integral_constant<bool,v>` where `v` is true iff none of the elements in the sequence satisfy the predicate `F`
 */
template< class Sequence, template<class ...> class F> struct none_of : none_of<as_sequence_t<Sequence>,F> {};
template< template<class ...> class F,class ... Args> struct none_of<sequence<Args...>,F > : variadic::none_of<F,Args...> {};

/**
 * \class count 
 * \brief Counts the number of elements in a sequence equal to a given one
 * \param Sequence The input sequence
 * \param T the type to be tested.
 * \return `count<...>::type` is `std::integral_constant<std::size_t,V>` where `V` is the number of elements in the sequence equal to `T`
 * \note The comparison is done with \ref tinympl::equal_to - it can be specialized.
 */
template<class Sequence,typename T> struct count : count<as_sequence_t<Sequence>,T > {};
template<typename T,class ... Args> struct count<sequence<Args...>,T > : variadic::count<T,Args...> {};

/**
 * \class count_if 
 * \brief Counts the number of elements which satisfy a given predicate
 * \param Sequence The input sequence
 * \param F The predicate - `F<T>::type::value` shall be convertible to bool
 * \return `count_if<...>::type` is `std::integral_constant<std::size_t,V>` where `V` is the number of elements in the sequence which satisfy the predicate `F`.
 */
template<class Sequence,template<class ... T> class F> struct count_if : count_if<as_sequence_t<Sequence>,F > {};
template<template<class ... T> class F,class ... Args> struct count_if<sequence<Args...>,F > : variadic::count_if<F,Args...> {};

/**
 * \class find 
 * \brief Compute the index of the first element in the sequence which is equal to the given type T
 * \param Sequence The input sequence
 * \param T The type to be tested
 * \return `find_if<...>::type` is `std::integral_constant<std::size_t,v>` where `v` is the 0-based index of the first element which is equal to `T`. If no such element exists, `v` is `size<Sequence>::value`.
 * \note The comparison is done with \ref tinympl::equal_to - it can be specialized
 */
template<class Sequence,typename T> struct find : find<as_sequence_t<Sequence>,T > {};
template<typename T,class ... Args> struct find<sequence<Args...>,T > : variadic::find<T,Args...> {};

/**
 * \class find_if Compute the index of a the first element in the sequence which satisfies a given predicate
 * \param Sequence The input sequence
 * \param F The test predicate - `F<T>::type::value` shall be convertible to bool
 * \return `find_if<...>::type` is `std::integral_constant<std::size_t,v>` where `v` is the 0-based index of the first element which satisfy `F`. If no such element exists, `v` is `size<Sequence>::value`.
 */
template<class Sequence,template<class ... T> class F> struct find_if : find_if<as_sequence_t<Sequence>,F > {};
template<template<class ... T> class F,class ... Args> struct find_if<sequence<Args...>, F> : variadic::find_if<F,Args...> {};

/** @} */

/**
 * \defgroup SeqModAlgs Modifying sequence operations
 * Algorithms which produce an output sequence
 * @{
 */

/**
 * \class copy 
 * \brief Copy the elements from the input sequence to the output sequence
 * \param SequenceIn The input sequence
 * \param Out The output sequence type - defaults to the same sequence kind of the input sequence
 * \return `copy<...>::type` is a type templated from `Out` which is constructed with the elements of SequenceIn.
 */
template<class SequenceIn,template<class ...> class Out = as_sequence<SequenceIn>::template rebind> struct copy : copy<as_sequence_t<SequenceIn>,Out> {};
template<template<class ...> class Out,class ... Args> struct copy<sequence<Args...>,Out>  : variadic::copy<Out,Args...> {};

/**
 * \class copy_if 
 * \brief Copy the elements of a given input sequence which satisfy a given predicate - the ordering is preserved.
 * \param SequenceIn The input sequence
 * \param F The test predicate - F<T>::type::value shall be convertible to bool
 * \param Out The output sequence type - defaults to the same sequence kind of the input sequence
 * \return `copy_if<...>::type` is a type templated from `Out` which is constructed with the elements of SequenceIn which satisfy the predicate `F`.
 */
template<class SequenceIn,template<class ... T> class F,template<class ...> class Out = as_sequence<SequenceIn>::template rebind> struct copy_if : copy_if<as_sequence_t<SequenceIn>,F,Out> {};
template<template<class ... T> class F,template<class ...> class Out,class ... Args> struct copy_if<sequence<Args...>,F,Out>  : variadic::copy_if<F,Out,Args...> {};

/**
 * \class copy_n 
 * \brief Copy the first n elements from the input sequence.
 * \param SequenceIn the input sequence
 * \param n The number of elements to be copied
 * \param Out The output sequence type, defaults to the same kind of the input sequence
 * \return `copy_n<...>::type` is a type templated from `Out` which is constructed with the first n types of the input sequence
 */
template<typename SequenceIn,std::size_t n,template<class ... > class Out = as_sequence<SequenceIn>::template rebind> struct copy_n : copy_n<as_sequence_t<SequenceIn>,n,Out> {};
template<std::size_t n,template<class ...> class Out,class ... Args> struct copy_n<sequence<Args...>,n,Out> : variadic::copy_n<n,Out,Args...> {};

/**
 * \class fill_n 
 * \brief Fills an output sequence with n equal elements
 * \param n The number of elements
 * \param T The type of the elements
 * \param Out The output sequence type
 * \return `fill_n<...>::type` is a type templated from `Out` constructed with n types equal to `T`
 */
template<std::size_t n,class T,template<class ...> class Out> struct fill_n : variadic::fill_n<n,T,Out> {};

/**
 * \class transpose 
 * \brief Transpose a sequence of sequences
 * \param SequenceOfSequences The sequence of sequences which shall be transposed
 * \param OutOuter The type of the outer output sequence
 * \param OutInner The type of the inner output sequence
 */
template<class SequenceOfSequences,
	template<class ...> class OutOuter = as_sequence<SequenceOfSequences>::template rebind,
	template<class ...> class OutInner = as_sequence<typename at<0,SequenceOfSequences>::type>::template rebind>
struct transpose : transpose< as_sequence_t<SequenceOfSequences>,OutOuter,OutInner> {};

template<class ... Sequences,template<class ...> class OutOuter,template<class ...> class OutInner> class transpose< sequence<Sequences...>, OutOuter,OutInner>
{
	static_assert(variadic::all_of<is_sequence,Sequences...>::value,"transpose: not all the elements of the main sequence are sequences");
	static_assert(sizeof...(Sequences) > 0,"transpose is undefined on empty sequences");
	enum {size = tinympl::size< typename variadic::at<0,Sequences...>::type>::value};
	
	static_assert(variadic::all_of<
		bind<equal_to,int_<size>,
			bind<tinympl::size,arg1> >::template eval, Sequences...>::value, "transpose: all the sequences must have the same size");
	
	template<std::size_t i,class ... Bound>
	struct impl
	{
		typedef OutInner< typename at<i,Sequences>::type ... > cur_t;
		typedef typename impl<i+1,Bound...,cur_t>::type type;
	};
	template<class ... Bound>
	struct impl<size,Bound...>
	{
		typedef OutOuter<Bound...> type;
	};
	
public:
	typedef typename impl<0>::type type;
};

/**
 * \class zip 
 * \brief Construct a single sequence by zipping together multiple sequences
 * \param OutSequence The type of the output sequence
 * \param ZipType The type of the inner sequence
 * \param Sequences... The input sequences
 */
template<template<class ...> class OutSequence,template<class ...> class ZipType,class ... Sequences> struct zip : transpose< sequence<Sequences...>,OutSequence,ZipType> {};

/**
 * \class transform 
 * \brief Transform an input sequence using a transform function
 * \param Sequence the input sequence
 * \param F The transform function. `F<T>::type` must be a valid expression
 * \param Out The output sequence type, defaults to the same kind of the input sequence
 * \return `transform<...>::type` is a type templated from `Out` which contains the transformed types
 */
template<class Sequence,template<class ... T> class F,template<class ... > class Out = as_sequence<Sequence>::template rebind> struct transform : transform<as_sequence_t<Sequence>,F,Out> {};
template<template<class ... T> class F,template<class ... > class Out,class ... Args> struct transform<sequence<Args...>,F,Out> : variadic::transform<F,Out,Args...> {};

/**
 * \class transform_many 
 * \brief Transform many input sequences using a function
 * \param F The transform function. `F<Args...>::type`, where `size<Args...>::value` is the number of input sequences, must be a valid expression.
 * \param Out The output sequence type
 * \param Sequences... The input sequences
 * \return `transform_many<...>::type` is a type templated from `Out` which contains the transformed types
 */
template<template<class ...> class F,template<class ...> class Out,class ... Sequences> struct transform_many
{
	template<class Seq> using F_t = typename copy<Seq,F>::type::type;
	
	typedef typename transform<
		typename transpose< sequence<Sequences...>,sequence,sequence>::type,
		F_t,
		Out>::type type;
};

/**
 * \class transform2 
 * \brief Transform two input sequences using a function
 * \param Sequence1 the first input sequence
 * \param Sequence2 the second input sequence
 * \param F The transform function. `F<T,U>::type` must be a valid expression
 * \param Out The output sequence type, defaults to the same kind of the input sequence
 * \return `transform2<...>::type` is a type templated from `Out` which contains the transformed types
 */
template<class Sequence1,class Sequence2,template<class ...> class F,template<class ...> class Out = as_sequence<Sequence1>::template rebind> struct transform2 : transform_many<F,Out,Sequence1,Sequence2> {};

/**
 * \class generate_n 
 * \brief Generate n elements using a given generator metafunction
 * \param n The number of elements to generate
 * \param Gen The generator. `Gen< std::integral_constant<int,i> >::type` must be a valid expression.
 * \param Out the output sequence type
 * \return `generate_n<...>::type` is a type templated from `Out` constructed with n elements generated with `Gen< int_<0> >, Gen< int_<1> >, ... Gen< int_<n-1> >`
 */
template<std::size_t n,template<class ...> class Gen,template<class ...> class Out> struct generate_n : variadic::generate_n<n,Gen,Out> {};

/**
 * \class remove 
 * \brief Remove all the elements equal to T from the input sequence
 * \param Sequence The input sequence
 * \param T The element to be removed
 * \param Out The type of the output sequence - defaults to the same kind of the input sequence
 * \return `remove<...>::type` is a type templated from `Out` which contains the new sequence
 * \note The comparison is done with \ref tinympl::equal_to - it can be specialized
 * 
 */
template< class Sequence,typename T, template<class ... > class Out = as_sequence<Sequence>::template rebind> struct remove : remove<as_sequence_t<Sequence>,T,Out> {};
template< typename T, template<class ... > class Out,class ... Args> struct remove<sequence<Args...>,T,Out> : variadic::remove<T,Out,Args...> {};

/**
 * \class remove_if 
 * \brief Remove the elements from the input sequence if they satisfy a given predicate
 * \param Sequence The input sequence
 * \param F The predicate, `F<T>::type::value` must be convertible to bool
 * \param Out The output sequence type, defaults to the same kind of the input sequence
 * \return `remove_if<...>::type` is a type templated from `Out` which contains the new sequence
 */
template<class Sequence,template<class ... T> class F,template<class ...> class Out = as_sequence<Sequence>::template rebind> struct remove_if : remove_if<as_sequence_t<Sequence>,F,Out> {};
template<template<class ... T> class F,template<class ...> class Out,class ... Args> struct remove_if<sequence<Args...>,F,Out> : variadic::remove_if<F,Out,Args...> {};

/**
 * \class replace 
 * \brief Replace all the elements in the input sequence equal to *Old* with *New*
 * \param SequenceIn The input sequence
 * \param Old The type to be replaced
 * \param New The new type 
 * \param Out The type of the output sequence, defaults to the same kind of the input sequence
 * \return `replace<...>::type` is a type templated from `Out`
 * \note The comparison is done with \ref tinympl::equal_to - it can be specialized
 * 
 */
template<class SequenceIn,class Old,class New,template<class ...> class Out = as_sequence<SequenceIn>::template rebind> struct replace : replace<as_sequence_t<SequenceIn>,Old,New,Out> {};
template<class Old,class New,template<class ...> class Out,class ... Args> struct replace<sequence<Args...>,Old,New,Out> : variadic::replace<Old,New,Out,Args...> {};

/**
 * \class replace_if 
 * \brief Replace all the elements in the input sequence which satisfy a given predicate with a given type T
 * \param SequenceIn The input sequence
 * \param F The predicate, `F<T>::type::value` must be convertible to bool
 * \param T The type used to replace the types
 * \param Out The type of the output sequence, defaults to the same kind of the input sequence
 * \return `replace_if<...>::type` is a type templated from `Out`
 */
template<class SequenceIn,template<class ... T> class F,class T,template<class ...> class Out = as_sequence<SequenceIn>::template rebind> struct replace_if : replace_if<as_sequence_t<SequenceIn>,F,T,Out> {};
template<template<class ... T> class F,class T,template<class ...> class Out,class ... Args> struct replace_if<sequence<Args...>,F,T,Out> : variadic::replace_if<F,T,Out,Args...> {};

/**
 * \class reverse 
 * \brief Reverse the input sequence
 * \param Sequence the input sequence
 * \param Out the output sequence type, defaults to the same kind of the input sequence type
 * \return `reverse<...>::type` is a type templated from `Out` which contains the reversed sequence
 */
template<class Sequence,template<class ...> class Out = as_sequence<Sequence>::template rebind> struct reverse : reverse< as_sequence_t<Sequence>, Out> {};
template<template<class ...> class Out,class ... Args> struct reverse<sequence<Args...>,Out> : variadic::reverse<Out,Args...> {};

/**
 * \class unique 
 * \brief Produces a sequence of unique elements from the input sequence, preserving the ordering.
 * \param Sequence The input sequence.
 * \param Out the output sequence type - defaults to the same kind of the input sequence.
 * \return `unique<...>::type` is a type templated from `Out` which contains the resulting sequence.
 * \note Only the first (leftmost) duplicate is mantained in the output sequence.
 */
template<class Sequence,template<class ...> class Out = as_sequence<Sequence>::template rebind> struct unique : unique<as_sequence_t<Sequence>,Out> {};
template<template<class ...> class Out,class ... Args> struct unique< sequence<Args...>,Out> : variadic::unique<Out,Args...> {};

/** @} */

/**
 * \defgroup SeqMaxMin Minimum/maximum operations
 * Algorithms which compute the minimum/maximum of a sequence and lexicographical compare
 * @{
 */

/**
 * \class min_element 
 * \brief Compute the index of the smallest element in a sequence
 * \param Sequence the input sequence
 * \param Cmp the comparator function; `Cmp<A,B>::type::value` must be convertible to bool. Defaults to \ref tinympl::less
 * \return `min_element<...>::type` is an `std::integral_constant<std::size_t,v>` where `v` is the 0-based index of the minimum element
 */
template<class Sequence,template<class ... > class Cmp = less> struct min_element : min_element<as_sequence_t<Sequence>,Cmp > {};
template<template<class ... > class Cmp,class ... Args> struct min_element<sequence<Args...>, Cmp> : variadic::min_element<Cmp,Args...> {};

/**
 * \class max_element 
 * \brief Compute the index of the largest element in a sequence
 * \param Sequence the input sequence
 * \param Cmp the comparator function; `Cmp<A,B>::type::value` must be convertible to bool. Defaults to \ref tinympl::less
 * \return `max_element<...>::type` is an `std::integral_constant<std::size_t,v>` where `v` is the 0-based index of the maximum element
 */
template<class Sequence,template<class ... > class Cmp = less> struct max_element : max_element<as_sequence_t<Sequence>,Cmp > {};
template<template<class ... > class Cmp,class ... Args> struct max_element<sequence<Args...>,Cmp > : variadic::max_element<Cmp,Args...> {};

/**
 * \class lexicographical_compare
 * \brief Compares two sequences using the given comparator
 * \param SequenceA the first sequence
 * \param SequenceB the second sequence
 * \param Comparator the comparator (default less)
 * \return An `std::integral_constant<int,v>` where `v` is -1 if the first sequence is lexicographically smaller than the second, 1 if the first is greater than the second and 0 if the two sequences are equal.
 */
template<class SequenceA,class SequenceB,template<class ...> class Comparator = less> struct lexicographical_compare : lexicographical_compare< as_sequence_t<SequenceA>, as_sequence_t<SequenceB>, Comparator> {};
template<class AHead,class ... ATail,class BHead,class ... BTail,template<class ...> class Comparator> struct lexicographical_compare< sequence<AHead,ATail...>, sequence<BHead,BTail...>, Comparator> : 
	std::integral_constant<int,
		(Comparator<AHead,BHead>::type::value ? 
			-1 :
		Comparator<BHead,AHead>::type::value ?
			1 : 
		lexicographical_compare<
			sequence<ATail...>,
			sequence<BTail...>,
			Comparator>::value) > {};

template<class ... As,template<class ...> class Comparator> struct lexicographical_compare< sequence<As...>, sequence<>, Comparator> : 
	std::integral_constant<int,1> {};
	
template<class ... Bs,template<class ...> class Comparator> struct lexicographical_compare< sequence<>, sequence<Bs...>, Comparator> : 
	std::integral_constant<int,-1> {};
	
template<template<class ...> class Comparator> struct lexicographical_compare< sequence<>, sequence<>, Comparator> : 
	std::integral_constant<int,0> {};

/** @} */

/**
 * \defgroup SeqSort Sorting operations
 * Algorithms which to sort a sequence.
 * @{
 */

/**
 * \class sort 
 * \brief Sort the input sequence according to a given comparison function
 * \param Sequence the input sequence
 * \param Out the output sequence type, defaults to the same kind of the input sequence type
 * \param Cmpl The comparison operator. `Cmp<A,B>::type::value` must be convertible to bool. The comparator must produce total ordering between elements. Defaults to \ref tinympl::less
 * \note The compile time complexity is O(N^2)
 * \return `sort<...>::type` is a type templated from `Out` which contains the sorted sequence
 */
template<class Sequence,template<class ...> class Out = as_sequence<Sequence>::template rebind,template<class ... > class Cmp = less> struct sort : sort<as_sequence_t<Sequence>,Out,Cmp> {};
template<template<class ... > class Cmp,template<class ...> class Out,class ... Args> struct sort<sequence<Args...>,Out,Cmp> : variadic::sort<Cmp,Out,Args...> {};

/** @} */

/**
 * \defgroup SeqSet Set operations (on unsorted sequences)
 * Algorithms which perform set operations.
 * \note Unlike the `std` counterparts, these algorithms do not require an ordering of the elements.
 * @{
 */

/**
 * \class unordered_equal 
 * \brief Determines whether it is possible to reorder the sequence `A` to match exactly the sequence `B`
 * \param SequenceA The first sequence
 * \param SequenceB The second sequence
 * \return `unordered_equal<A,B>::type` is a `std::integral_constant<bool,v>` where `v` is true only if the two sequences are equal (except for the ordering of the elements)
 * \note Compile time complexity is O(N^2)
 */
template<class SequenceA,class SequenceB>
struct unordered_equal : unordered_equal< as_sequence_t<SequenceA>, as_sequence_t<SequenceB> > {};

namespace detail
{
template<class SequenceA,class SequenceB> struct unordered_equal_impl;

template<class ... As,class ... Bs> struct unordered_equal_impl<sequence<As...>,sequence<Bs...> >
{
	template<class T> struct check_t
	{
		typedef std::integral_constant<bool,
			variadic::count<T,As...>::type::value ==
			variadic::count<T,Bs...>::type::value> type;
	};
	
	typedef typename logical_and<
		typename variadic::all_of< check_t, As...>::type,
		typename variadic::all_of< check_t, Bs...>::type >::type type;
};
}
template<class ... As,class ... Bs> struct unordered_equal<sequence<As...>,sequence<Bs...> > : 
	detail::unordered_equal_impl< sequence<As...>, sequence<Bs...> >::type {};

/**
 * \class set_union 
 * \brief Computes the union of two sets
 * \param SequenceA The sequence which represents the first set - duplicates are ignored
 * \param SequenceB The sequence which represents the second set - duplicates are ignored
 * \param Out the output sequence type - defaults to the same kind of `SequenceA`
 * \return A type templated from `Out` which contains the resulting sequence
 * \note If `seq<A...>` and `seq<B...>` are the two input sequences, the resulting sequence is constructed as `unique< seq<A...,B...> >`
 */
template<class SequenceA,class SequenceB,template<class ...> class Out = as_sequence<SequenceA>::template rebind> struct set_union : set_union< as_sequence_t<SequenceA>, as_sequence_t<SequenceB>,Out> {};
template<class ... Ts,class ... Us,template<class ...> class Out> struct set_union<sequence<Ts...>,sequence<Us...>,Out> : variadic::unique<Out,Ts...,Us...> {};

/**
 * \class set_intersection 
 * \brief Computes the intersection between two sets
 * \param SequenceA The sequence which represents the first set - duplicates are ignored
 * \param SequenceB The sequence which represents the second set - duplicates are ignored
 * \param Out the output sequence type - defaults to the same kind of `SequenceA`
 * \return A type templated from `Out` which contains the resulting sequence
 * \note The ordering of the first sequence is preserved
 */
template<class SequenceA,class SequenceB,template<class ...> class Out = as_sequence<SequenceA>::template rebind> struct set_intersection : set_intersection< as_sequence_t<SequenceA>, as_sequence_t<SequenceB>,Out> {};
template<class ... Ts,class ... Us,template<class ...> class Out> struct set_intersection<sequence<Ts...>,sequence<Us...>,Out> : variadic::copy_if< bind<variadic::count,arg1,Us...>::template eval_t, Out, Ts...> {};

/**
 * \class set_difference 
 * \brief Computes the difference between two sets
 * \param SequenceA The sequence which represents the first set - duplicates are ignored
 * \param SequenceB The sequence which represents the second set - duplicates are ignored
 * \param Out the output sequence type - defaults to the same kind of `SequenceA`
 * \return A type templated from `Out` which contains the resulting sequence
 * \note The ordering of the first sequence is preserved
 */
template<class SequenceA,class SequenceB,template<class ...> class Out = as_sequence<SequenceA>::template rebind> struct set_difference : set_difference< as_sequence_t<SequenceA>, as_sequence_t<SequenceB>,Out> {};
template<class ... Ts,class ... Us,template<class ...> class Out> struct set_difference<sequence<Ts...>,sequence<Us...>,Out> : variadic::remove_if< bind<variadic::count,arg1,Us...>::template eval_t, Out, Ts...> {};

/**
 * \class is_unique 
 * \brief Determines whether the input sequence contains only unique elements
 * \param Sequence the input sequence
 * \return `is_unique<...>::type` is a `std::integral_constant<bool,v>` where `v` is true iff the input sequence contains no duplicates
 * \note Unlike `std::sort`, the input sequence is not required to be sorted, but the compile time complexity is O(N^2)
 * \note The comparison is done with \ref tinympl::equal_to - it can be specialized
 * 
 */
template<class Sequence> struct is_unique : is_unique<as_sequence_t<Sequence> > {};
template<class ... Args> struct is_unique<sequence<Args...> > : variadic::is_unique<Args...> {};

/** @} */

/**
 * \defgroup SeqFold Folding operations
 * Algorithms which perform reduction operations on a sequence.
 * @{
 */

/**
 * \class left_fold 
 * \brief Collapses a sequence starting from left using a functor.
 * \param Sequence The input sequence
 * \param F The functor; `F<T,U>` must be a valid expression
 * \return A type which is the result of `F( ... F(F(A1,A2),A3) .. )`
 */
template<class Sequence,template<class ...> class F> struct left_fold : left_fold< as_sequence_t<Sequence>, F> {};
template<class ... Ts,template<class ...> class F> struct left_fold<sequence<Ts...>, F> : variadic::left_fold<F,Ts...> {};

/**
 * \class accumulate 
 * \brief Alias for left_fold.
 * \see { left_fold }
 */
template<class Seq,template<class ... > class F> struct accumulate : left_fold<Seq,F> {};

/**
 * \class right_fold 
 * \brief Collapses a sequence starting from right using a functor.
 * \param Sequence The input sequence
 * \param F The functor; `F<T,U>` must be a valid expression
 * \return A type which is the result of `F(A1, F(A2,F(A3, ... ) ) )`
 */
template<class Sequence,template<class ...> class F> struct right_fold : right_fold< as_sequence_t<Sequence>, F> {};
template<class ... Ts,template<class ...> class F> struct right_fold<sequence<Ts...>, F> : variadic::right_fold<F,Ts...> {};

/** @} */

/** @} */

}

#endif // TINYMPL_ALGORITHM_HPP 
