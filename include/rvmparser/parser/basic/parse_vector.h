#ifndef RVMPARSER_PARSER_BASIC_PARSE_VECTOR_H
#define RVMPARSER_PARSER_BASIC_PARSE_VECTOR_H

namespace RvmParser
{
namespace Parser
{
namespace Basic
{
template<typename T, size_t D, template<typename, size_t> typename VECTOR> requires bool VECTOR_Contract = requires(VECTOR<T, D> v)
{
	{ v[0] } -> T&;
	{ v[D-1] } -> T&;
};
}
}
}

#endif
