#ifndef LEVEL_SPACE_HH_
#define LEVEL_SPACE_HH_

#include "Level.hh"

class SolarSystemObject;


class LevelSpace : public Level
{
public:
  LevelSpace(Scene * scene, SolarSystemObject * planet);
  void Start() override;
  
private:
  SolarSystemObject * _solar_system_object;
  double _planet_size;
  double _planet_position_start;
};

#endif
