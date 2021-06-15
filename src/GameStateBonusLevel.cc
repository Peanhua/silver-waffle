#include "GameStateBonusLevel.hh"
#include "BonusLevel.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


GameStateBonusLevel::GameStateBonusLevel(GameStateGame * mainlevel, double enemy_difficulty, double warp_fuel_bonus)
  : GameStateGame(mainlevel->GetGameStats()),
    _mainlevel(mainlevel),
    _enemy_difficulty(enemy_difficulty),
    _warp_fuel_bonus(warp_fuel_bonus)
{
  SetupLevels();  
  OnLevelChanged();

  _scene->GetPlayer()->CopyUpgrades(*mainlevel->GetScene()->GetPlayer());
}

  
void GameStateBonusLevel::SetupLevels()
{
  _levels.clear();
  auto level = new BonusLevel(_scene, _enemy_difficulty, _warp_fuel_bonus);
  _levels.push_back(level);
  OnLevelChanged();
}


void GameStateBonusLevel::OnQuit()
{
  _mainlevel->GetScene()->GetPlayer()->CopyUpgrades(*_scene->GetPlayer());
  _mainlevel->OnLivesUpdated();
}
