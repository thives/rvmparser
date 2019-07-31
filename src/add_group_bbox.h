#ifndef ADD_GROUP_BBOX_H
#define ADD_GROUP_BBOX_H

#include <cassert>
#include "common.h"
#include "store_visitor.h"

namespace AddGroupBBox
{
template<typename T> concept bool DATATYPE_FLOATING_Contract = requires
{
	std::is_floating_point<T>::value;
	{ sizeof(T) == 8};
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class AddGroupBBox : public StoreVisitor<T>
{
public:
	void init(class Store::Store<T>& store) override
	{
		this->store = &store;
		//stack = (Store::Group<T>**)arena.alloc(sizeof(Store::Group<T>*)*store.groupCountAllocated());
		stack_p = 0;
	}

	void geometry(struct Store::Geometry<T>* geometry) override
	{
		assert(stack_p);
		const auto & M = geometry->M_3x4;
		(stack[stack_p - 1]->group.bboxWorld).engulf(geometry->bboxWorld);
	}

	void beginGroup(struct Store::Group<T>* group)  override
	{
		group->group.bboxWorld = LinAlg::BBox<3, T>::createEmptyBBox3f();
		stack[stack_p] = group;
		stack_p++;
	}

	void EndGroup() override
	{
		assert(stack_p);
		stack_p--;
		auto & bbox = stack[stack_p]->group.bboxWorld;
		if (!bbox.isEmpty() && 0 < stack_p) {
			auto & parentBox = stack[stack_p - 1]->group.bboxWorld;
			parentBox.engulf(bbox);
		}
	}

protected:
	Store::Store<T>* store = nullptr;
	//Arena arena;
	Store::Group<T>** stack = nullptr;
	unsigned stack_p = 0;
};

}

#endif
