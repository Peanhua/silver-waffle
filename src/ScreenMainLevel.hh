#ifndef SCREEN_MAINLEVEL_HH_
#define SCREEN_MAINLEVEL_HH_

#include "ScreenLevel.hh"


class ScreenMainLevel : public ScreenLevel
{
public:
  ScreenMainLevel(ScreenLevel * parent);
  
  void SetupLevels() override;
};

#endif
