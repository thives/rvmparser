#ifndef PARSER_H
#define PARSER_H

#include <chrono>

namespace Parser
{



template<typename HEADING, typename HEADER, typename STRING> concept bool HEADING_Contract = requires(
	HEADING h, 
	unsigned version, 
	S&& info, 
	S&& note, 
	const const std::chrono::system_clock::time_point& timestamp, 
	S&& author,
	S&& contentEncoding)
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
