#ifndef STORE_VISITOR_H
#define STORE_VISITOR_H
#include <vector>
#include "store.h"

template<typename T> concept bool DATATYPE_FLOATING_Contract = requires
{
	std::is_floating_point<T>::value;
	{ sizeof(T) == 8};
};

template<typename T> requires
DATATYPE_FLOATING_Contract<T>
class StoreVisitor
{
public:
  virtual void init(Store::Store<T>& store) {}

  virtual bool done() { return true; }

  virtual void beginFile(Store::Group<T>* group) {}

  virtual void endFile() {}

  virtual void beginModel(Store::Group<T>* group) {}

  virtual void endModel() {};

  virtual void beginGroup(Store::Group<T>* group) {}

  virtual void doneGroupContents(Store::Group<T>* group) {}

  virtual void EndGroup() {}

  virtual void beginChildren(Store::Group<T>* container) {}

  virtual void endChildren() {}

  virtual void beginAttributes(Store::Group<T>* container) {}

  virtual void attribute(const char* key, const char* val) {}

  virtual void endAttributes(Store::Group<T>* container) {}

  virtual void beginGeometries(Store::Group<T>* container) {}

  virtual void geometry(Store::Geometry<T>* geometry) {}

  virtual void endGeometries() {}

};

#endif
