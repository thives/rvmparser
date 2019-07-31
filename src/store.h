#ifndef STORE_H
#define STORE_H

#include <list>
#include <cstdint>
#include "common.h"
#include "lin_alg.h"

namespace Store
{
template<typename T> concept bool DATATYPE_FLOATING_Contract = requires
{
	std::is_floating_point<T>::value;
	{ sizeof(T) == 8};
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Group;

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Geometry;

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Contour
{
public:
	T* vertices;
	T* normals;
	size_t vertices_n;
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Polygon
{
public:
	Contour<T>* contours;
	size_t contours_n;
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Triangulation {
public:
	T* vertices = nullptr;
	T* normals = nullptr;
	T* texCoords = nullptr;
	size_t* indices = 0;
	size_t vertices_n = 0;
	size_t triangles_n = 0;
	unsigned id = {0};
	T error = {0};
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Connection
{
public:
	enum class Flags : char8_t {
		None = 0,
		HasCircularSide = 1<<0,
		HasRectangularSide = 1<<1
	};

	Connection<T>* next = nullptr;
	Geometry<T>* geo[2] = { nullptr, nullptr };
	unsigned offset[2];
	LinAlg::Vec<3, T> p;
	LinAlg::Vec<3, T> d;
	unsigned temp;
	Flags flags = Flags::None;

	void setFlag(Flags flag) { flags = (Flags)((uint8_t)flags | (uint8_t)flag); }
	bool hasFlag(Flags flag) { return (uint8_t)flags & (uint8_t)flag; }

};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Geometry
{
public:
	enum class Kind
	{
		Pyramid,
		Box,
		RectangularTorus,
		CircularTorus,
		EllipticalDish,
		SphericalDish,
		Snout,
		Cylinder,
		Sphere,
		Line,
		FacetGroup
	};
	Geometry<T>* next = nullptr; // Next geometry in the list of geometries in group.
	Triangulation<T>* triangulation = nullptr;
	Geometry<T>* next_comp = nullptr; // Next geometry in list of geometries of this composite

	Connection<T>* connections[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	const char* colorName = nullptr;
	void * clientData = nullptr;
	uint32_t color = 0x202020u;

	Kind kind;
	unsigned id;

	LinAlg::Mat<3,4,T> M_3x4;
	LinAlg::BBox<3,T> bboxLocal;
	LinAlg::BBox<3, T> bboxWorld;
	T sampleStartAngle = {0};
  union {
    struct {
      float bottom[2];
      float top[2];
      float offset[2];
      float height;
    } pyramid;
    struct {
      float lengths[3];
    } box;
    struct {
      float inner_radius;
      float outer_radius;
      float height;
      float angle;
    } rectangularTorus;
    struct {
      float offset;
      float radius;
      float angle;
    } circularTorus;
    struct {
      float baseRadius;
      float height;
    } ellipticalDish;
    struct {
      float baseRadius;
      float height;
    } sphericalDish;
    struct {
      float offset[2];
      float bshear[2];
      float tshear[2];
      float radius_b;
      float radius_t;
      float height;
    } snout;
    struct {
      float radius;
      float height;
    } cylinder;
    struct {
      float diameter;
    } sphere;
    struct {
      float a, b;
    } line;
    struct {
      struct Polygon<T>* polygons;
      uint32_t polygons_n;
    } facetGroup;
  };
};
/*
template<typename T>
struct ListHeader
{
  T* first;
  T* last;

  void clear()
  {
    first = last = nullptr;
  }

  void insert(T* item)
  {
    if (first == nullptr) {
      first = last = item;
    }
    else {
      last->next = item;
      last = item;
    }
  }
};
*/
class Attribute
{
	Attribute* next = nullptr;
	const char* key = nullptr;
	const char* val = nullptr;
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Group
{
public:
	Group() {}

	enum class Kind
	{
		File,
		Model,
		Group
	};

	enum struct Flags
	{
		None = 0,
		ClientFlagStart = 1
	};

	Group<T>* next = nullptr;
	std::list<Group<T>> groups;
	std::list<Attribute> attributes;

	Kind kind = Kind::Group;
	Flags flags = Flags::None;

	void setFlag(Flags flag) { flags = (Flags)((unsigned)flags | (unsigned)flag); }
	void unsetFlag(Flags flag) { flags = (Flags)((unsigned)flags & (~(unsigned)flag)); }
	bool hasFlag(Flags flag) const { return ((unsigned)flags & (unsigned)flag) != 0; }

  union {
    struct {
      const char* info;
      const char* note;
      const char* date;
      const char* user;
      const char* encoding;
    } file;
    struct {
      const char* project;
      const char* name;
    } model;
    struct {
      std::list<Geometry<T>> geometries;
      const char* name;
      LinAlg::BBox<3,T> bboxWorld;
      uint32_t material;
      int32_t id = 0;
      T translation[3];
      uint32_t clientTag;     // For use by passes to stuff temporary info
    } group;
  };

};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
struct DebugLine
{
  DebugLine<T>* next = nullptr;
  T a[3];
  T b[3];
  uint32_t color = 0xff0000u;
};


class StoreVisitor;

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class Store
{
public:
  Store();

  Geometry<T>* newGeometry(Group<T>* parent);

  Geometry<T>* cloneGeometry(Group<T>* parent, const Geometry<T>* src);

  Group<T>* getDefaultModel();

  Group<T>* newGroup(Group<T>* parent, typename Group<T>::Kind kind);

  Group<T>* cloneGroup(Group<T>* parent, const Group<T>* src);

  Group<T>* findRootGroup(const char* name);

  Attribute* getAttribute(Group<T>* group, const char* key);

  Attribute* newAttribute(Group<T>* group, const char* key);

  void addDebugLine(T* a, T* b, uint32_t color);

  Connection<T>* newConnection();

  void apply(StoreVisitor* visitor);

  size_t groupCount_() const { return numGroups; }
  size_t groupCountAllocated() const { return numGroupsAllocated; }
  size_t leafCount() const { return numLeaves; }
  size_t emptyLeafCount() const { return numEmptyLeaves; }
  size_t nonEmptyNonLeafCount() const { return numNonEmptyNonLeaves; }
  size_t geometryCount_() const { return numGeometries; }
  size_t geometryCountAllocated() const { return numGeometriesAllocated; }

  const char* errorString() const { return error_str; }
  void setErrorString(const char* str);

  Group<T>* getFirstRoot() { return roots.first; }
  Connection<T>* getFirstConnection() { return connections.first; }
  DebugLine<T>* getFirstDebugLine() { return debugLines.first; }

  //Arena<T> arena;
  //Arena<T> arenaTriangulation;
  struct Stats* stats = nullptr;
  struct Connectivity* conn = nullptr;

  //StringInterning strings;

  void updateCounts();

  void forwardGroupIdToGeometries();

protected:
	size_t numGroups = 0;
	size_t numGroupsAllocated = 0;
	size_t numLeaves = 0;
	size_t numEmptyLeaves = 0;
	size_t numNonEmptyNonLeaves = 0;
	size_t numGeometries = 0;
	size_t numGeometriesAllocated = 0;

	const char* error_str = nullptr;

	void updateCountsRecurse(Group<T>* group);

	void apply(StoreVisitor* visitor, Group<T>* group);

	std::list<Group<T>> roots;
	std::list<DebugLine<T>> debugLines;
	std::list<Connection<T>> connections;
};
}
#endif
