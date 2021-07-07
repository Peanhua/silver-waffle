#ifndef OBJECT_PLANET_HH_
#define OBJECT_PLANET_HH_

#include "ObjectMovable.hh"

class Image;
class SolarSystemObject;


class ObjectPlanet : public ObjectMovable
{
public:
  ObjectPlanet(Scene * scene, SolarSystemObject * solar_system_object, Image * planet_texture, double planet_size);
  ~ObjectPlanet();

  void AddPlanetRing(float start, float end);
  SolarSystemObject * GetSolarSystemObject() const;
  
private:
  SolarSystemObject * _solar_system_object;
};

#endif
