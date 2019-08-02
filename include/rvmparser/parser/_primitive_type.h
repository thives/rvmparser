#ifndef RVMPARSER_PARSER__PRIMITIVE_TYPE_H
#define RVMPARSER_PARSER__PRIMITIVE_TYPE_H

namespace RvmParser
{
namespace Parser
{
enum class PrimitiveType
{
	Pyramid = 1,
	Box = 2,
	RectangularTorus = 3,
	CircularTorus = 4,
	EllipticalDish = 5,
	SphericalDish = 6,
	Snout = 7,
	Cylinder = 8,
	Sphere = 9,
	Line = 10,
	FacetGroup = 11
};

template<typename PRIMITIVE> concept bool PRIMITIVE_Contract = requires
{
	{ PRIMITIVE::type } -> PrimitiveType;
};
}
}

#endif
