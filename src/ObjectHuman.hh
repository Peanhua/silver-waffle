#ifndef OBJECT_HUMAN_HH_
#define OBJECT_HUMAN_HH_

#include "ObjectMovable.hh"

class ObjectHuman : public ObjectMovable
{
public:
  ObjectHuman(Scene * scene, unsigned int random_seed);

  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
};


#endif
