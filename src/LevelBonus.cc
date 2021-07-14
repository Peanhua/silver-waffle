#include "LevelBonus.hh"

LevelBonus::LevelBonus(Scene * scene, double enemy_difficulty, double warp_fuel_bonus)
  : Level(scene)
{
  _name = "Bonus Level " + std::to_string(enemy_difficulty) + "/" + std::to_string(warp_fuel_bonus);

  LoadConfig("Data/Level-Bonus");
}
