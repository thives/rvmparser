#ifndef RVMPARSER_PARSER_COMPOSITE_PARSE_PYRAMID_H
#define RVMPARSER_PARSER_COMPOSITE_PARSE_PYRAMID_H

#include "parser/_parser_role.h"
#include "parser/_primitive_type.h"

namespace RvmParser
{
namespace Parser
{
namespace Primitive
{
template<typename VECTOR, typename T, size_t D> concept bool VECTOR_Contract = requires(VECTOR<T, D> v)
{
	{ v[0] } -> T&;
	{ v[D-1] } -> T&;
};

template<typename PRIMITIVE> concept bool PRIMITIVE_PYRAMIDE_Contract = requires
{
	requires PRIMITIVE_Contract<PRIMITIVE>;
	requires PRIMITIVE::type == PrimitiveType::Pyramid;
};

template<typename PYRAMID, typename VECTOR> concept bool PYRAMID_Contract = requires(PYRAMID p)
{
	requires VECTOR_Contract<VECTOR>
	std::array<T, 2> m_bottom;
	std::array<T, 2> m_top;
	std::array<T, 2> m_offset;
	T m_height;
	{ p.bottom() } -> VECTOR;
};

template<typename PYRAMID, typename PRIMITIVE> requires
PYRAMID_Contract<PYRAMID> &&
PRIMITIVE_Contract<PRIMITIVE>
class ParsePyramid
{
public:
	using value_type = PYRAMID;
	ParsePyramid(const unsigned char* data, FLOATING_PARSER* floatingParser) {}
		m_floatingParser(floatingParser), 
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
