#ifndef SCREEN_BONUS_LEVEL_HH_
#define SCREEN_BONUS_LEVEL_HH_

#include "ScreenMainLevel.hh"


class ScreenBonusLevel : public ScreenMainLevel
{
public:
  ScreenBonusLevel(ScreenMainLevel * mainlevel, double enemy_difficulty, double warp_fuel_bonus);
  
  void SetupLevels() override;

private:
  ScreenMainLevel * _mainlevel;
  double _enemy_difficulty;
  double _warp_fuel_bonus;

  void OnQuit() override;
};

#endif
