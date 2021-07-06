#ifndef SCREEN_BONUS_LEVEL_HH_
#define SCREEN_BONUS_LEVEL_HH_

#include "ScreenMainLevel.hh"


class ScreenBonusLevel : public ScreenMainLevel
{
public:
  ScreenBonusLevel(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus);
  
  void SetupLevels() override;

private:
  double _enemy_difficulty;
  double _warp_fuel_bonus;
};

#endif
