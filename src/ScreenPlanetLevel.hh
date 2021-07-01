#ifndef SCREEN_PLANET_LEVEL_HH_
#define SCREEN_PLANET_LEVEL_HH_

#include "ScreenBonusLevel.hh"


class ScreenPlanetLevel : public ScreenBonusLevel
{
public:
  ScreenPlanetLevel(ScreenMainLevel * mainlevel);

  void SetupLevels() override;

private:
};

#endif
