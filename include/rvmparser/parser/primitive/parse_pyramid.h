#ifndef RVMPARSER_PARSER_COMPOSITE_PARSE_PYRAMID_H
#define RVMPARSER_PARSER_COMPOSITE_PARSE_PYRAMID_H

#include "parser/_parser_role.h"
#include "parser/_primitive_type.h"
#include "utility.h"

namespace RvmParser
{
namespace Parser
{
namespace Primitive
{
template<template<typename,size_t> typename VECTOR, typename T, size_t D> concept bool VECTOR_Contract = requires
{
	requires IS_NCONSTRUCTIBLE_Contract<VECTOR, T, D>;
};

template<typename PRIMITIVE> concept bool PRIMITIVE_PYRAMIDE_Contract = requires
{
	requires PRIMITIVE_Contract<PRIMITIVE>;
	requires PRIMITIVE::type == PrimitiveType::Pyramid;
};

template<typename PYRAMID, template<typename,size_t> typename VECTOR, typename T> concept bool PYRAMID_Contract = requires(
	PYRAMID<VECTOR<T, 2>> p,
	VECTOR<T, 2>& bottom,
	VECTOR<T, 2>& top,
	VECTOR<T, 2>& offset,
	T height)
{
	requires VECTOR_Contract<VECTOR, T, 2>
	{ p.bottom() } -> VECTOR<T, 2>&;
	{ p.top() } -> VECTOR<T, 2>&;
	{ p.offset() } -> VECTOR<T, 2>&;
	{ p.height() } -> T&;
	{ PYRAMID(bottom, top, offset, height) };
};

template<typename PYRAMID, typename PRIMITIVE, typename T, template<typename,size_t> typename VECTOR, template<size_t,typename> typename FLOATING_PARSER, typename VECTOR_PARSER> requires
PYRAMID_Contract<PYRAMID, VECTOR, T> &&
PRIMITIVE_PYRAMIDE_Contract<PRIMITIVE> &&
PARSER_Contract<FLOATING_PARSER> &&
PARSER_Contract<VECTOR_PARSER>
class ParsePyramid
{
public:
	using value_type = PYRAMID;
	ParsePyramid(const unsigned char* data, FLOATING_PARSER* floatingParser, VECTOR_PARSER* vectorParser) {}
		m_floatingParser(floatingParser), 
		m_vectorParser(vectorParser),
		m_data(data), 
		m_next(nullptr),
		m_value(0), 
		m_executed(false) {}
	value_type value()
	{
		return m_executed ? m_value : execute();
	}
	unsigned char* next()
	{
		if (!m_executed) {
			execute();
		}
		return m_next;
	}
	void operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
	}
protected:
	FLOATING_PARSER* m_floatingParser;
	VECTOR_PARSER* m_vectorParser;
protected:
	const unsigned char* m_data;
	unsigned char* m_next;
	value_type m_value;
	bool m_executed;
	value_type execute()
	{
	}
};
}
}
}

#endif
