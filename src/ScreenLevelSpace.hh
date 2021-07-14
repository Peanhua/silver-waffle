#ifndef SCREEN_LEVEL_SPACE_HH_
#define SCREEN_LEVEL_SPACE_HH_

#include "ScreenLevel.hh"


class ScreenLevelSpace : public ScreenLevel
{
public:
  ScreenLevelSpace(ScreenLevel * parent);
  
  void SetupLevels() override;
};

#endif
