#ifndef SCREEN_PLANET_LEVEL_HH_
#define SCREEN_PLANET_LEVEL_HH_

#include "ScreenLevel.hh"

class SolarSystemObject;


class ScreenPlanetLevel : public ScreenLevel
{
public:
  ScreenPlanetLevel(ScreenLevel * parent, SolarSystemObject * planet);

  void Tick(double deltatime) override;
  void SetupLevels() override;

private:
  SolarSystemObject * _planet;
};

#endif
