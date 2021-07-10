#ifndef PLANET_LEVEL_HH_
#define PLANET_LEVEL_HH_

#include "Level.hh"

class SolarSystemObject;


class PlanetLevel : public Level
{
public:
  PlanetLevel(Scene * scene, SolarSystemObject * planet);
};

#endif
