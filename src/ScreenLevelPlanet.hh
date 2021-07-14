#ifndef SCREEN_LEVEL_PLANET_HH_
#define SCREEN_LEVEL_PLANET_HH_

#include "ScreenLevel.hh"

class SolarSystemObject;


class ScreenLevelPlanet : public ScreenLevel
{
public:
  ScreenLevelPlanet(ScreenLevel * parent, SolarSystemObject * planet);

  void Tick(double deltatime) override;
  void SetupLevels() override;

private:
  SolarSystemObject * _planet;
};

#endif
