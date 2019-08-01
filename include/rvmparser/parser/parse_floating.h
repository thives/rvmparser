#ifndef RVMPARSER_PARSER_PARSE_FLOATING_H
#define RVMPARSER_PARSER_PARSE_FLOATING_H

#include <memory>

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
	{ p(data) };
	{ P(data) };
};

// Context definition.
template<size_t N, typename INTEGER_PARSER> requires
PARSE_FLOATING_SIZE_Contract<N> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, N>
class ParseFloating
{
public:
	using value_type = 
		typename std::conditional<(N == sizeof(float)), float, 
		typename std::conditional<(N == sizeof(double)), double, long double>::type>::type;
	ParseFloating(const unsigned char* data, INTEGER_PARSER* integerParser) : 
		m_integerParser(integerParser), m_data(data), m_value(0), m_executed(false) {}
	value_type value()
	{
		return m_executed ? m_value : execute();
	}
	void operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
	}
protected: // Roles.
	INTEGER_PARSER* m_integerParser;
protected:
	const unsigned char* m_data;
	value_type m_value;
	bool m_executed;
	value_type execute()
	{
		typename INTEGER_PARSER<N>::value_type bytes = (*m_integerParser)(m_data)->value();
		m_value = *reinterpret_cast<value_type*>(&bytes);
		m_executed = true;
		return m_value;
	}
};
}
}

#endif
