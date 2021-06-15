#ifndef BONUS_LEVEL_HH_
#define BONUS_LEVEL_HH_

#include "Level.hh"

class BonusLevel : public Level
{
public:
  BonusLevel(Scene * scene, double enemy_difficulty, double warp_fuel_bonus);

  bool   IsFinished()   const override;
  double GetTotalTime() const override;
};

#endif
