#include "ScreenBonusLevel.hh"
#include "BonusLevel.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


ScreenBonusLevel::ScreenBonusLevel(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus)
  : ScreenMainLevel(parent),
    _enemy_difficulty(enemy_difficulty),
    _warp_fuel_bonus(warp_fuel_bonus)
{
}


void ScreenBonusLevel::SetupLevels()
{
  _levels.clear();
  auto level = new BonusLevel(_scene, _enemy_difficulty, _warp_fuel_bonus);
  _levels.push_back(level);

  ScreenLevel::SetupLevels();
}
