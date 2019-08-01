#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <array>

namespace Primitives
{
template<typename T> concept bool DATATYPE_FLOATING_Contract = requires
{
	std::is_floating_point<T>::value;
};

template<typename T, typename VECTOR> concept bool VECTOR_Contract = requires(VECTOR<T> v)
{
	requires DATATYPE_FLOATING_Contract<T>;
	/*{ v[0] -> T& };
	{ v[1] -> T& };
	{ v[2] -> T& };
	{ v[3] -> T& };*/
};

template<typename T, typename LIST> concept bool LIST_Contract = requires(LIST<T> l, T elem)
{
	{ l.push_back(elem) };
	{ l.begin() } -> LIST<T>::iterator;
	{ l.end() } -> LIST<T>::iterator;
};

template<typename T, typename VECTOR, typename LIST> requires
VECTOR_Contract<T, VECTOR> &&
LIST_Contract<std::pair<VECTOR, VECTOR>, LIST>
class Contour
{
protected:
	LIST<std::pair<VECTOR<T>, VECTOR<T>>> m_vertices;
};

template<typename T, typename VECTOR, typename LIST, typename CONTOUR> concept bool CONTOUR_Contract = requires(CONTOUR<T, VECTOR, LIST> v)
{
	requires DATATYPE_FLOATING_Contract<T>;
	requires LIST_Contract<T, LIST>;
	requires VECTOR_Contract<T, VECTOR>;
	// Contour requirements.
};

template<typename T, typename VECTOR, typename CONTOUR, typename LIST> requires
CONTOUR_Contract<T, VECTOR, LIST, CONTOUR>
class Polygon
{
protected:
	LIST<CONTOUR<T, VECTOR, LIST>> m_contours;
};

template<typename T>
class Pyramid
{
protected:
	std::array<T, 2> m_bottom;
	std::array<T, 2> m_top;
	std::array<T, 2> m_offset;
	T m_height;
};

template<typename T>
class Box
{
protected:
	std::array<T, 3> m_lengths;
};

template<typename T>
class RectangularTorus
{
protected:
	T m_innerRadius;
	T m_outerRadius;
	T m_height;
	T m_angle;
};

template<typename T>
class CiruclarTorus
{
protected:
	T m_offset;
	T m_radius;
	T m_angle;
};

template<typename T>
class EllipticalDish
{
protected:
	T m_baseRadius;
	T m_height;
};

template<typename T>
class SphericalDish
{
protected:
	T m_baseRadius;
	T m_height;
};

template<typename T>
class Snout
{
protected:
	std::array<T, 2> m_offset;
	std::array<T, 2> m_bShear;
	std::array<T, 2> m_tShear;
	T m_radiusB;
	T m_radiusT;
	T m_height;
};

template<typename T>
class Cylinder
{
protected:
	T m_radius;
	T m_height;
};

template<typename T>
class Sphere
{
protected:
	T m_diameter;
};

template<typename T>
class Line
{
protected:
	T m_a;
	T m_b;
};

template<typename T, typename VECTOR, typename CONTOUR, typename LIST, typename POLYGON> concept bool POLYGON_Concept = requires(POLYGON<T, VECTOR, CONTOUR, LIST> p)
{
	requires CONTOUR_Contract<T, VECTOR, LIST, CONTOUR>;
	// Polygon requirements.
};

template<typename T, typename VECTOR, typename CONTOUR, typename POLYGON, typename LIST> requires
DATATYPE_FLOATING_Contract<T> &&
LIST_Contract<POLYGON, LIST> &&
POLYGON_Concept<T, VECTOR, CONTOUR, LIST, POLYGON>
class FacetGroup
{
protected:
	LIST<POLYGON<T, VECTOR, CONTOUR, LIST>> m_polygons;
};
}

#endif
