#ifndef SCREEN_PLANET_LEVEL_HH_
#define SCREEN_PLANET_LEVEL_HH_

#include "ScreenLevel.hh"


class ScreenPlanetLevel : public ScreenLevel
{
public:
  ScreenPlanetLevel(ScreenLevel * parent);

  void Tick(double deltatime) override;
  void SetupLevels() override;

private:
};

#endif
