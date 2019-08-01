#ifndef PARSER_H
#define PARSER_H

#include <chrono>

namespace Parser
{
template<typename STRING> concept bool STRING_Contract = requires(STRING s, size_t i, const char* c)
{
	{ s[i] } -> char&;
	{ STRING(c, i) };
};

template<typename HEADER, typename STRING> concept bool HEADER_Contract = requires(H h)
{
	requires STRING_Contract<STRING>;
	{ HEADER::name() } -> const char*;
	{ h.version() } -> unsigned;
	{ h.size() } -> size_t; // Chunk size in bytes.
};

template<typename HEADING, typename HEADER, typename STRING> concept bool HEADING_Contract = requires(
	HEADING h, 
	unsigned version, 
	const S& info, 
	const S& note, 
	const const std::chrono::system_clock::time_point& timestamp, 
	const S& author,
	const S& contentEncoding)
{
	requires STRING_Contract<S>;
	requires HEADER_Contract<HEADER, STRING>;
	{ h.header() } -> const HEADER&;
	{ h.info() } -> const S&;
	{ h.note() } -> const S&;
	{ h.timestamp() } -> const std::chrono::system_clock::time_point&;
	{ h.author() } -> const S&;
	{ h.contentEncoding() } -> const S&; // TODO turn into enum class or something else.
	{ H(version, info, note, timestamp, author, contentEncoding) };
};

template<typename INTEGER_PARSER, size_t N> concept bool INTEGER_PARSER_Contract = requires(INTEGER_PARSER<N> p, const unsigned char* data)
{
	require N == 4;
	{ p.value() } -> INTEGER_PARSER<N>::value_type;
	{ P(data) };
};

template<typename INTEGER_PARSER, size_t N, typename STRING> requires
INTEGER_PARSER_Contract<INTEGER_PARSER, N> &&
STRING_Contract<STRING>
class ParseString
{
public:
	using value_type = STRING;
	ParseString(const unsigned char* data) : m_data(data), m_executed(false) {}
	const value_type& value() const
	{
		return m_executed ? m_value : execute();
	}
protected:
	
protected:
	const unsigned char* m_data;
	mutable value_type m_value;
	bool m_executed;
	const value_type& execute()
	{
		size_t length = 4*INTEGER_PARSER<N>(m_data).value(); // The format specifies length as number of 32 bit chunks.
		m_value = value_type(reinterpret_cast<const char*>(m_data+N), length);
		m_executed = true;
		return m_value;
	}
};

template<typename HEADER, typename STRING> requires
HEADER_Contract<HEADER, STRING>
class ParseHeader
{
public:
	ParseHeader(const unsigned char* data)
	{
		
	}
protected:
};

template<typename HEADING, typename HEADER, typename STRING> requires
HEADING_Contract<HEADING, HEADER, STRING>
class ParseHeading
{
public:
protected:
};

template<typename C>
class ParseRVM
{
public:
	ParseRVM(const char* data, size_t length) // TODO: Convert to iterators.
	{
		
	}
};
}

#endif
