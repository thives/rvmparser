#ifndef RVMPARSER_PARSER__HEADER_ROLE_H
#define RVMPARSER_PARSER__HEADER_ROLE_H

namespace RvmParser
{
namespace Parser
{
template<typename STRING> concept bool HEADER_STRING_Contract = requires(STRING string, size_t i, const char* c)
{
	{ STRING(c, i) };
	{ string.length() } -> size_t;
};

template<typename HEADER, typename STRING> concept bool HEADER_Contract = requires(
	HEADER<STRING> header,
	STRING&& name,
	size_t size)
{
	require HEADER_STRING_Contract<STRING>;
	{ header.name() } -> const STRING&;
	{ header.size() } -> size_t; // Chunk size in bytes.
	{ HEADER(name, version, size) };
};
}
}

#endif
