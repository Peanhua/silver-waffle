#ifndef OBJECT_CLOUD_HH_
#define OBJECT_CLOUD_HH_

#include "ObjectMovable.hh"


class ObjectCloud : public ObjectMovable
{
public:
  ObjectCloud(Scene * scene, unsigned int random_seed);
};

#endif
