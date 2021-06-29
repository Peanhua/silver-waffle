#ifndef OBJECT_PLANET_HH_
#define OBJECT_PLANET_HH_

#include "ObjectMovable.hh"

class Image;


class ObjectPlanet : public ObjectMovable
{
public:
  ObjectPlanet(Scene * scene, Image * planet_texture, double planet_size);
  ~ObjectPlanet();

  void AddPlanetRing(float start, float end);
};

#endif
