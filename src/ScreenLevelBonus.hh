#ifndef SCREEN_LEVEL_BONUS_HH_
#define SCREEN_LEVEL_BONUS_HH_

#include "ScreenLevelSpace.hh"


class ScreenLevelBonus : public ScreenLevelSpace
{
public:
  ScreenLevelBonus(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus);
  
  void SetupLevels() override;

private:
  double _enemy_difficulty;
  double _warp_fuel_bonus;
};

#endif
