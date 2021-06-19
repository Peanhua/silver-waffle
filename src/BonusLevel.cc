#include "BonusLevel.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>


BonusLevel::BonusLevel(Scene * scene, double enemy_difficulty, double warp_fuel_bonus)
  : Level(scene)
{
  assert(enemy_difficulty == enemy_difficulty);
  assert(warp_fuel_bonus == warp_fuel_bonus);

  _name = "Bonus Level " + std::to_string(enemy_difficulty) + "/" + std::to_string(warp_fuel_bonus);
  
  auto levelconfig = AssetLoader->LoadJson("Data/Level-Bonus");
  assert(levelconfig);
  if(levelconfig)
    {
      auto spawns = (*levelconfig)["spawns"];
      assert(spawns.is_array());
      for(auto spawn : spawns.array_items())
        _program.push_back(new ProgramEntry(spawn));
    }
}
