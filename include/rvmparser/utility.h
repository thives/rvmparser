#ifndef RVMPARSER_UTILITY_H
#define RVMPARSER_UTILITY_H

#include <utility>
#include <type_traits>

namespace RvmParser
{
template<template<typename, size_t...> typename TYPE, typename T, size_t ... D>
class is_nconstructible
{
public:
	static constexpr bool value = is_nconstructible::m_is_constructible(std::make_index_sequence<is_nconstructible::m_flatten()>{});
protected:
	template<size_t>
	using type_t = T;
	template<size_t ... I>
	static constexpr bool m_is_constructible(std::index_sequence<I...>) { return std::is_constructible<TYPE<T, D...>, type_t<I>...>::value; }
	static constexpr size_t m_flatten() { return ((D * ...)); }
};

template<template<typename, size_t...> typename TYPE, typename T, size_t ... D> concept bool IS_NCONSTRUCTIBLE_Contract = requires
{
	requires is_nconstructible<TYPE, T, D...>::value;
};

template<typename ITERATOR, typename VALUE_TYPE> concept bool ITERATOR_Contract = requires(ITERATOR it)
{
	{ it++ } -> ITERATOR;
	{ *it } -> VALUE_TYPE&;
};

template<typename T> concept bool ITERATES_Contract = requires(T obj)
{
	requires ITERATOR_Contract<typename T::iterator, typename T::value_type>;
	{ obj.begin() } -> typename T::iterator;
	{ obj.end() } -> typename T::iterator;
};
}

#endif
