#ifndef RVMPARSER_PARSER_BASIC_PARSE_ARRAY_H
#define RVMPARSER_PARSER_BASIC_PARSE_ARRAY_H

#include "parser/_parser_role.h"

namespace RvmParser
{
namespace Parser
{
namespace Basic
{
template<typename ARRAY, typename T, size_t N, typename PARSER> requires
PARSER_Contract<PARSER<T>>
class ParseArray
{
public:
	using value_type = ARRAY;
	ParseArray(const unsigned char* data, PARSER* parser) :
		m_parser(parser),
		m_data(data),
		m_next(nullptr),
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
	PARSER* m_parser;
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
