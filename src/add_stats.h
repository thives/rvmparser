#ifndef ADD_STATS_H
#define ADD_STATS_H

#include "store_visitor.h"

namespace Stats
{
struct Stats
{
  size_t group_n = 0;
  size_t geometry_n = 0;
  size_t pyramid_n = 0;
  size_t box_n = 0;
  size_t rectangular_torus_n = 0;
  size_t circular_torus_n = 0;
  size_t elliptical_dish_n = 0;
  size_t spherical_dish_n = 0;
  size_t snout_n = 0;
  size_t cylinder_n = 0;
  size_t sphere_n = 0;
  size_t facetgroup_n = 0;
  size_t facetgroup_triangles_n = 0;
  size_t facetgroup_quads_n = 0;
  size_t facetgroup_polygon_n = 0;
  size_t facetgroup_polygon_n_contours_n = 0;
  size_t facetgroup_polygon_n_vertices_n = 0;
  size_t line_n = 0;
};

template<typename T> concept bool DATATYPE_FLOATING_Contract = requires
{
	std::is_floating_point<T>::value;
	{ sizeof(T) == 8};
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class AddStats : public StoreVisitor<T>
{
public:
	void init(class Store::Store<T>& store) override
	{
		//store.stats = store.arena.alloc<Stats>();
		stats = store.stats;
	}
	void beginGroup(struct Store::Group<T>* group) override
	{
		stats->group_n++;
	}
	void geometry(struct Store::Geometry<T>* geometry) override
	{
		stats->geometry_n++;
		switch (geometry->kind) {
			case Store::Geometry<T>::Kind::Pyramid: 
				stats->pyramid_n++; 
				break;
			case Store::Geometry<T>::Kind::Box: 
				stats->box_n++; 
				break;
			case Store::Geometry<T>::Kind::RectangularTorus: 
				stats->rectangular_torus_n++; 
				break;
			case Store::Geometry<T>::Kind::CircularTorus: 
				stats->circular_torus_n++; 
				break;
			case Store::Geometry<T>::Kind::EllipticalDish: 
				stats->elliptical_dish_n++; 
				break;
			case Store::Geometry<T>::Kind::SphericalDish: 
				stats->spherical_dish_n++; 
				break;
			case Store::Geometry<T>::Kind::Snout: 
				stats->snout_n++; 
				break;
			case Store::Geometry<T>::Kind::Cylinder: 
				stats->cylinder_n++; 
				break;
			case Store::Geometry<T>::Kind::Sphere: 
				stats->sphere_n++; 
				break;
			case Store::Geometry<T>::Kind::FacetGroup:
				stats->facetgroup_n++;
				for (unsigned p = 0; p < geometry->facetGroup.polygons_n; p++) {
					auto & poly = geometry->facetGroup.polygons[p];
					if (poly.contours_n == 1 && poly.contours[0].vertices_n == 3) {
						stats->facetgroup_triangles_n++;
					}
					else if (poly.contours_n == 1 && poly.contours[0].vertices_n == 4) {
						stats->facetgroup_quads_n++;
					}
					else {
						stats->facetgroup_polygon_n++;
						stats->facetgroup_polygon_n_contours_n += geometry->facetGroup.polygons[p].contours_n;
						for (unsigned c = 0; c < geometry->facetGroup.polygons[p].contours_n; c++) {
							stats->facetgroup_polygon_n_vertices_n += geometry->facetGroup.polygons[p].contours[c].vertices_n;
						}
					}
				}
				break;
			case Store::Geometry<T>::Kind::Line: 
				stats->line_n++; 
				break;
			default:
				assert(false && "Unhandled primitive type");
				break;
		}
	}
	bool done() override
	{
		return true;
	}
protected:
	struct Stats* stats = nullptr;

};
}
#endif
