#ifndef OBJECT_PLANET_GROUND_HH_
#define OBJECT_PLANET_GROUND_HH_

#include "Object.hh"

class Image;


class ObjectPlanetGround : public Object
{
public:
  ObjectPlanetGround(Scene * scene, const glm::vec2 & size, Image * texture_map);

  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
};

#endif
