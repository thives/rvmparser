#ifndef RVMPARSER_PARSER_PARSE_STRING_H
#define RVMPARSER_PARSER_PARSE_STRING_H

namespace RvmParser
{
namespace Parser
{
template<typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires(INTEGER_PARSER<N> p, const unsigned char* data)
{
	require N == 4;
	{ p.value() } -> INTEGER_PARSER<N>::value_type;
};

template<typename STRING> concept bool STRING_Contract = requires(STRING s, size_t i, const char* c)
{
	{ s[i] } -> char&;
	{ s = STRING(c, i) };
};

template<typename STRING, typename INTEGER_PARSER, size_t N> requires
STRING_Contract<STRING> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, N>
class ParseString
{
public:
	using value_type = STRING;
	ParseString(const unsigned char* data, INTEGER_PARSER<N>* integerParser) : m_integerParser(integerParser), m_data(data), m_executed(false) {}
	const value_type& value()
	{
		return m_executed ? m_value : execute();
	}
	void operator()(const unsigned char* data)
	{
		m_data = data;
		m_executed = false;
	}
protected:
	INTEGER_PARSER<N>* m_integerParser;
protected:
	const unsigned char* m_data;
	value_type m_value;
	bool m_executed;
	const value_type& execute()
	{
		size_t length = 4*(*m_integerParser)(m_data).value(); // The format specifies length as number of 32 bit chunks.
		m_value = value_type(reinterpret_cast<const char*>(m_data+N), length);
		m_executed = true;
		return m_value;
	}
};
}
}

#endif
