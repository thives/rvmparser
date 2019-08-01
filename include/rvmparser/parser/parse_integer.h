#ifndef RVMPARSER_PARSER_PARSE_INTEGER_H
#define RVMPARSER_PARSER_PARSE_INTEGER_H

#include <type_traits>
#include <functional>
#include <utility>

namespace RvmParser
{
namespace Parser
{
// Role contract definitions and other constraints.
template<size_t N> concept bool PARSE_INTEGER_SIZE_Contract = requires
{
	requires N <= sizeof(unsigned long long);
	requires N > 0;
};

// Context definition.
template<size_t N> requires
PARSE_INTEGER_SIZE_Contract<N>
class ParseInteger
{
public:
	using value_type = 
		typename std::conditional<(N <= sizeof(unsigned char)), unsigned char, 
		typename std::conditional<(N <= sizeof(unsigned short)), unsigned short,
		typename std::conditional<(N <= sizeof(unsigned)), unsigned,
		typename std::conditional<(N <= sizeof(unsigned long)), unsigned long, unsigned long long>::type>::type>::type>::type;
	ParseInteger(const unsigned char* data) : m_data(data), m_value(0), m_executed(false) {}
	value_type value()
	{
		return m_executed ? m_value : execute();
	}
	void operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
	}
protected:
	const unsigned char* m_data;
	value_type m_value;
	bool m_executed;
	value_type execute()
	{
		std::invoke([this]<size_t ... I>(std::index_sequence<I...>) {
			m_value = ((m_data[(sizeof...(I)-I-1)] << (8*I)) | ...);
		}, std::make_index_sequence<N>{});
		m_executed = true;
		return m_value;
	}
};
}
}

#endif
