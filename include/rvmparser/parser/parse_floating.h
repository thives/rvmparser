#ifndef RVMPARSER_PARSER_PARSE_FLOATING_H
#define RVMPARSER_PARSER_PARSE_FLOATING_H

#include <memory>
#include "parse_integer.h"

namespace RvmParser
{
namespace Parser
{
// Role contract definitions and other constraints.
template<size_t N> concept bool PARSE_FLOATING_SIZE_Contract = requires
{
	requires (N == sizeof(long double) || N == sizeof(double) || N == sizeof(float));
	requires N > 0;
};

template<typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires(INTEGER_PARSER<N> p, const unsigned char* data)
{
	{ p.value() } -> INTEGER_PARSER<N>::value_type;
	{ P(data) };
};

// Context definition.
template<size_t N, typename INTEGER_PARSER = ParseInteger> requires
PARSE_FLOATING_SIZE_Contract<N> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, N>
class ParseFloating
{
public:
	using value_type = 
		typename std::conditional<(N == sizeof(float)), float, 
		typename std::conditional<(N == sizeof(double)), double, long double>::type>::type;
	ParseFloating(std::unique_ptr<INTEGER_PARSER>&& integerParser) : 
		m_integerParser(integerParser), m_value(0), m_executed(false) {}
	ParseFloating(const unsigned char* data, std::unique_ptr<INTEGER_PARSER<N>>&& integerParser = std::make_unique<INTEGER_PARSER<N>>(data)) : 
		m_integerParser(integerParser), m_value(0), m_executed(false) {}
	value_type value() const 
	{
		return m_executed ? m_value : execute();
	}
protected:
	std::unique_ptr<INTEGER_PARSER> m_integerParser;
	mutable value_type m_value;
	bool m_executed;
	value_type execute()
	{
		typename ParseInteger<N>::value_type bytes = m_integerParser->value();
		m_value = *reinterpret_cast<value_type*>(&bytes);
		m_executed = true;
		return m_value;
	}
};
}
}

#endif
