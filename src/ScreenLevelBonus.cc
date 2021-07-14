#include "ScreenLevelBonus.hh"
#include "LevelBonus.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


ScreenLevelBonus::ScreenLevelBonus(ScreenLevel * parent, double enemy_difficulty, double warp_fuel_bonus)
  : ScreenLevelMain(parent),
    _enemy_difficulty(enemy_difficulty),
    _warp_fuel_bonus(warp_fuel_bonus)
{
}


void ScreenLevelBonus::SetupLevels()
{
  _levels.clear();
  auto level = new LevelBonus(_scene, _enemy_difficulty, _warp_fuel_bonus);
  _levels.push_back(level);

  ScreenLevel::SetupLevels();
}
