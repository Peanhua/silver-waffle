#ifndef SCREEN_LEVEL_MAIN_HH_
#define SCREEN_LEVEL_MAIN_HH_

#include "ScreenLevel.hh"


class ScreenLevelMain : public ScreenLevel
{
public:
  ScreenLevelMain(ScreenLevel * parent);
  
  void SetupLevels() override;
};

#endif
