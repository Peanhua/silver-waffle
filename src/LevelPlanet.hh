#ifndef LEVEL_PLANET_HH_
#define LEVEL_PLANET_HH_

#include "Level.hh"

class SolarSystemObject;


class LevelPlanet : public Level
{
public:
  LevelPlanet(Scene * scene, SolarSystemObject * planet);
};

#endif
