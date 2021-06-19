#include "BonusLevel.hh"
#include <cassert>


BonusLevel::BonusLevel(Scene * scene, double enemy_difficulty, double warp_fuel_bonus)
  : Level(scene)
{
  assert(enemy_difficulty == enemy_difficulty);
  assert(warp_fuel_bonus == warp_fuel_bonus);

  _name = "Bonus Level " + std::to_string(enemy_difficulty) + "/" + std::to_string(warp_fuel_bonus);
  
  auto e = new ProgramEntry();
  e->SetStartTime(0.0);
  e->SetStopTime(30.0);
  e->SetSpawnInterval(0.5);
  _program.push_back(e);
}


bool BonusLevel::IsFinished() const
{
  return false;
}
