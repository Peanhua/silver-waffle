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

  auto myplr = _scene->GetPlayer();
  auto mainplr = _mainlevel->GetScene()->GetPlayer();
  myplr->CopyUpgrades(*mainplr);
  myplr->SetHealth(mainplr->GetHealth());
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
  auto myplr = _scene->GetPlayer();
  auto mainplr = _mainlevel->GetScene()->GetPlayer();
  mainplr->CopyUpgrades(*myplr);
  mainplr->SetHealth(myplr->GetHealth());
  _mainlevel->OnLivesUpdated();

  if(_mainlevel->GetGameStats()->GetLives() == 0)
    _mainlevel->Quit();
}
