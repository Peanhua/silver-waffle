#ifndef LEVEL_BONUS_HH_
#define LEVEL_BONUS_HH_

#include "Level.hh"

class LevelBonus : public Level
{
public:
  LevelBonus(Scene * scene, double enemy_difficulty, double warp_fuel_bonus);
};

#endif
