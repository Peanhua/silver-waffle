#ifndef LEVEL_MAIN_HH_
#define LEVEL_MAIN_HH_

#include "Level.hh"

class SolarSystemObject;


class LevelMain : public Level
{
public:
  LevelMain(Scene * scene, SolarSystemObject * planet);
  void Start() override;
  
private:
  SolarSystemObject * _solar_system_object;
  double _planet_size;
  double _planet_position_start;
};

#endif
