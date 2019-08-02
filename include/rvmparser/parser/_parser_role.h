#ifndef RVMPARSER_PARSER__PARSER_ROLE_H
#define RVMPARSER_PARSER__PARSER_ROLE_H

namespace RvmParser
{
namespace Parser
{
template<typename PARSER> concept bool PARSER_Contract = requires(PARSER p, const unsigned char* data)
{
	{ p.value() } -> PARSER::value_type;
	{ p.next() } -> const unsigned char*;
	{ p(data) };
	{ P(data) };
};
}
}

#endif
