#ifndef OBJECT_PLANET_ATMOSPHERE_HH_
#define OBJECT_PLANET_ATMOSPHERE_HH_

#include "Object.hh"


class ObjectPlanetAtmosphere : public Object
{
public:
  ObjectPlanetAtmosphere(Scene * scene, const glm::vec2 & size, const glm::vec3 & bottom_color, const glm::vec3 & top_color);
};

#endif
