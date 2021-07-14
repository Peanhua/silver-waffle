#ifndef SCREEN_LEVEL_BONUS_HH_
#define SCREEN_LEVEL_BONUS_HH_

#include "ScreenLevelMain.hh"


class ScreenLevelBonus : public ScreenLevelMain
{
public:
  ScreenLevelBonus(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus);
  
  void SetupLevels() override;

private:
  double _enemy_difficulty;
  double _warp_fuel_bonus;
};

#endif
