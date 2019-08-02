#ifndef RVMPARSER_PARSER_PARSE_HEADING_H
#define RVMPARSER_PARSER_PARSE_HEADING_H

#include <chrono>
#include <memory>
#include "parser/_parser_role.h"
#include "parser/composite/_header_role.h"

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

template<typename HEADING, typename HEADER, typename STRING> concept bool HEADING_Contract = requires(
	HEADING heading,
	HEADER&& header,
	STRING&& info,
	STRING&& note,
	const std::chono::system_clock::time_point& timestamp,
	STRING&& author,
	STRING&& contentEncoding)
{
	require STRING_Contract<STRING>;
	require HEADER_Contract<HEADER, STRING>;
	{ heading.header() } -> const HEADER<STRING>&;
	{ heading.info() } -> const STRING&;
	{ heading.note() } -> const STRING&;
	{ heading.timestamp() } -> const std::chrono::system_clock::time_point&;
	{ heading.author() } -> const STRING&;
	{ heading.contentEncoding() } -> const STRING&;
	{ HEADING(header, info, note, timestamp, author, contentEncoding) };
};

template<typename STRING_PARSER> concept bool STRING_PARSER_Contract = requires
{
	requires PARSER_Contract<STRING_PARSER>;
};

// Context definition.
template<typename HEADING, typename HEADER, typename STRING, typename STRING_PARSER> requires
HEADING_Contract<HEADING> &&
STRING_Contract<STRING> &&
STRING_PARSER_Contract<STRING>
class ParseHeading
{
public:
	using value_type = HEADING;
	ParseHeading(const unsigned char* data, HEADER<STRING>&& header, STRING_PARSER* stringParser) : 
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
		// Check header for correctness.
		unsigned char* data = m_data;
		STRING info = (*m_stringParser)(data).value();
		data = m_stringParser->next();
		STRING note = (*m_stringParser)(data).value();
		data = m_stringParser->next();
		STRING timestampString = (*m_stringParser)(data).value();
		data = m_stringParser->next();
		STRING author = (*m_stringParser)(data).value();
		data = m_stringParser->next();
		STRING contentEncoding;
		if (m_header.version >= 2) {
			contentEncoding = (*m_stringParser)(data).value();
		}
		std::stringstream ss;
		ss << timestampString;
		std::chrono::system_clock::time_point timestamp;
		std::chrono::from_stream(ss, "%A%n%B%n%d%n%T%n%Y", timestamp);
		if (ss.bad()) {
			throw std::runtime_error("Error: Failed parsing date and time.");
		}
		m_value = std::make_unique<value_type>(m_header, info, note, timestamp, author, contentEncoding);
		m_executed = false;
		m_next = m_stringParser->next();
		return m_value;
	}
};
}
}
}

#endif
