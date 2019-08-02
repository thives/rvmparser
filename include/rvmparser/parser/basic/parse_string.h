#ifndef RVMPARSER_PARSER_PARSE_STRING_H
#define RVMPARSER_PARSER_PARSE_STRING_H

#include "parser/_parser_role.h"

namespace RvmParser
{
namespace Parser
{
namespace Basic
{
// Role contract definitions and other constraints.
template<typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires
{
	requires PARSER_Contract<INTEGER_PARSER<N>>;
	requires N == 4;
};

template<typename STRING> concept bool STRING_Contract = requires(STRING s, size_t i, const char* c)
{
	{ s[i] } -> char&;
	{ s = STRING(c, i) };
};

// Context definition.
template<typename STRING, typename INTEGER_PARSER, size_t N> requires
STRING_Contract<STRING> &&
INTEGER_PARSER_Contract<INTEGER_PARSER, N>
class ParseString
{
public:
	using value_type = STRING;
	ParseString(const unsigned char* data, INTEGER_PARSER<N>* integerParser) : 
		m_integerParser(integerParser), 
		m_data(data),
		m_next(nullptr),
		m_executed(false) {}
	const value_type& value()
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
protected: // Roles.
	INTEGER_PARSER<N>* m_integerParser;
protected:
	const unsigned char* m_data;
	unsigned char* m_next;
	value_type m_value;
	bool m_executed;
	const value_type& execute()
	{
		unsigned char* data = m_data;
		size_t length = 4*(*m_integerParser)(data).value(); // The format specifies length as number of 32 bit chunks.
		data = m_integerParser->next();
		m_value = value_type(reinterpret_cast<const char*>(data), length);
		m_executed = true;
		m_next = data + length;
		return m_value;
	}
};
}
}
}

#endif
