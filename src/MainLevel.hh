#ifndef MAIN_LEVEL_HH_
#define MAIN_LEVEL_HH_

#include "Level.hh"

class SolarSystemObject;


class MainLevel : public Level
{
public:
  MainLevel(Scene * scene, const SolarSystemObject * planet);

  void   Start()              override;
  bool   IsFinished()   const override;
  
private:
  const SolarSystemObject * _solar_system_object;
  double _planet_size;
  double _planet_position_start;
};

#endif
