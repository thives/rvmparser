#ifndef RVMPARSER_PARSER_COMPOSITE_PARSE_PYRAMID_H
#define RVMPARSER_PARSER_COMPOSITE_PARSE_PYRAMID_H

#include "parser/_parser_role.h"
#include "parser/_primitive_type.h"

namespace RvmParser
{
namespace Parser
{
namespace Primitive
{
template<typename PYRAMID> requires
PYRAMID_Contract<PYRAMID>
class ParsePyramid
{
public:
	using value_type = PYRAMID;
	constexpr PrimitiveType = PrimitiveType::Pyramid;
	ParsePyramid(const unsigned char* data) {}
protected:
};
}
}
}

#endif
