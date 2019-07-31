#ifndef STORE_h
#define STORE_H

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

template<typename T> concept bool DATATYPE_INTEGER_Contract = requires
{
	std::is_integral<T>::value;
	{ sizeof(T) == 4};
};

struct Group;
struct Geometry;

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
struct Contour
{
  T* vertices;
  T* normals;
  size_t vertices_n;
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
struct Polygon
{
  Contour<T>* contours;
  size_t contours_n;
};

template<typename T, typename I> requires
DATATYPE_FLOATING_Contract<T> &&
DATATYPE_INTEGER_Contract<I>
struct Triangulation {
  T* vertices = nullptr;
  T* normals = nullptr;
  T* texCoords = nullptr;
  I* indices = 0;
  size_t vertices_n = 0;
  size_t triangles_n = 0;
  I id = {0};
  T error = {0};
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
struct Connection
{
  enum class Flags : FlagsType {
    None = 0,
    HasCircularSide =     1<<0,
    HasRectangularSide =  1<<1
  };

  Connection* next = nullptr;
  Geometry* geo[2] = { nullptr, nullptr };
  unsigned offset[2];
  Vec<3, T> p;
  Vec<3, T> d;
  unsigned temp;
  Flags flags = Flags::None;

  void setFlag(Flags flag) { flags = (Flags)((uint8_t)flags | (uint8_t)flag); }
  bool hasFlag(Flags flag) { return (uint8_t)flags & (uint8_t)flag; }

};


struct Geometry
{
  enum struct Kind
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
  Geometry* next = nullptr;                 // Next geometry in the list of geometries in group.
  Triangulation* triangulation = nullptr;
  Geometry* next_comp = nullptr;            // Next geometry in list of geometries of this composite

  Connection* connections[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
  const char* colorName = nullptr;
  void * clientData = nullptr;
  uint32_t color = 0x202020u;

  Kind kind;
  unsigned id;

  Mat3x4f M_3x4;
  BBox3f bboxLocal;
  BBox3f bboxWorld;
  float sampleStartAngle = 0.f;
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
      struct Polygon* polygons;
      uint32_t polygons_n;
    } facetGroup;
  };
};

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

struct Attribute
{
  Attribute* next = nullptr;
  const char* key = nullptr;
  const char* val = nullptr;
};


struct Group
{
  Group() {}

  enum struct Kind
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

  Group* next = nullptr;
  ListHeader<Group> groups;
  ListHeader<Attribute> attributes;

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
      ListHeader<Geometry> geometries;
      const char* name;
      BBox3f bboxWorld;
      uint32_t material;
      int32_t id = 0;
      float translation[3];
      uint32_t clientTag;     // For use by passes to stuff temporary info
    } group;
  };

};


struct DebugLine
{
  DebugLine* next = nullptr;
  float a[3];
  float b[3];
  uint32_t color = 0xff0000u;
};


class StoreVisitor;

class Store
{
public:
  Store();

  Geometry* newGeometry(Group* parent);

  Geometry* cloneGeometry(Group* parent, const Geometry* src);

  Group* getDefaultModel();

  Group* newGroup(Group * parent, Group::Kind kind);

  Group* cloneGroup(Group* parent, const Group* src);

  Group* findRootGroup(const char* name);

  Attribute* getAttribute(Group* group, const char* key);

  Attribute* newAttribute(Group* group, const char* key);

  void addDebugLine(float* a, float* b, uint32_t color);

  Connection* newConnection();

  void apply(StoreVisitor* visitor);

  unsigned groupCount_() const { return numGroups; }
  unsigned groupCountAllocated() const { return numGroupsAllocated; }
  unsigned leafCount() const { return numLeaves; }
  unsigned emptyLeafCount() const { return numEmptyLeaves; }
  unsigned nonEmptyNonLeafCount() const { return numNonEmptyNonLeaves; }
  unsigned geometryCount_() const { return numGeometries; }
  unsigned geometryCountAllocated() const { return numGeometriesAllocated; }

  const char* errorString() const { return error_str; }
  void setErrorString(const char* str);

  Group* getFirstRoot() { return roots.first; }
  Connection* getFirstConnection() { return connections.first; }
  DebugLine* getFirstDebugLine() { return debugLines.first; }

  Arena arena;
  Arena arenaTriangulation;
  struct Stats* stats = nullptr;
  struct Connectivity* conn = nullptr;

  StringInterning strings;

  void updateCounts();

  void forwardGroupIdToGeometries();

private:
  unsigned numGroups = 0;
  unsigned numGroupsAllocated = 0;
  unsigned numLeaves = 0;
  unsigned numEmptyLeaves = 0;
  unsigned numNonEmptyNonLeaves = 0;
  unsigned numGeometries = 0;
  unsigned numGeometriesAllocated = 0;

  const char* error_str = nullptr;

  void updateCountsRecurse(Group* group);

  void apply(StoreVisitor* visitor, Group* group);

  ListHeader<Group> roots;
  ListHeader<DebugLine> debugLines;
  ListHeader<Connection> connections;
  
};
}
#endif
