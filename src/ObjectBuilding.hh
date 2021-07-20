#ifndef OBJECT_BUILDING_HH_
#define OBJECT_BUILDING_HH_

#include "Object.hh"


class ObjectBuilding : public Object
{
public:
  ObjectBuilding(Scene * scene, unsigned int random_seed, unsigned int type);
};

#endif
