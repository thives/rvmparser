#ifndef RVMPARSER_PARSER_PARSE_MODL_H
#define RVMPARSER_PARSER_PARSE_MODL_H

#include <memory>
#include "parser/_parser_role.h"
#include "parser/_header_role.h"

namespace RvmParser
{
namespace Parser
{
namespace Composite
{
// Role contract definitions and other constraints.
template<typename STRING> concept bool STRING_Contract = requires(STRING string, size_t i, const char* c)
{
	{ STRING(c, i) };
	{ string.length() } -> size_t;
};

template<typename MODL, typename HEADER, typename STRING> concept bool MODL_Contract = requires(
	MODL<HEADER, STRING> modl,
	HEADER<STRING> header,
	STRING&& project,
	STRING&& name)
{
	requires STRING_Contract<STRING>;
	requires HEADER_Contract<HEADER<STRING>>;
	{ modl.header() } -> const HEADER<STRING>&;
	{ modl.project() } -> const STRING&;
	{ modl.name() } -> const STRING&;
	{ MODL<HEADER, STRING>(std::move(header), project, name) };
};

template<typename STRING_PARSER> concept bool STRING_PARSER_Contract = requires(STRING_PARSER p)
{
	requires PARSER_Contract<STRING_PARSER>;
};

// Context definition.
template<typename MODL, typename HEADER, typename STRING, typename STRING_PARSER, typename INTEGER_PARSER> requires
MODL_Contract<MODL, STRING> &&
HEADER_Contract<HEADER, STRING> &&
STRING_PARSER_Contract<STRING_PARSER>
class ParseModl 
{
public:
	using value_type = MODL<HEADER, STRING>;
	constexpr size_t string
	ParseModl(const unsigned char* data, HEADER<STRING>&& header, STRING_PARSER* stringParser) :
		m_stringParser(stringParser),
		m_data(data),
		m_next(nullptr),
		m_header(std::move(header)),
		m_executed(false) {}
	std::unique_ptr<value_type>& value()
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
	void operator()(const unsigned char* data, HEADER&& header)
	{
		m_data = data;
		m_header = std::move(header);
		m_executed = false;
	}
protected: // Roles.
	STRING_PARSER* m_stringParser;
protected:
	const unsigned char* m_data;
	unsigned char* m_next;
	HEADER<STRING> m_header;
	std::unique_ptr<value_type> m_value;
	bool m_executed;
	std::unique_ptr<value_type>& execute()
	{
		unsigned char* data = m_data;
		STRING project = (*m_stringParser)(data).value();
		data = m_stringParser->next();
		STRING name = (*m_stringParser)(data).value();
		data = m_stringParser->next();
		m_value = std::make_unique<value_type>(std::move(m_header), project, name);
		m_executed = true;
		return m_value;
	}
};
}
}
}

#endif
