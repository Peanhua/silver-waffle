#include "ScreenBonusLevel.hh"
#include "BonusLevel.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


ScreenBonusLevel::ScreenBonusLevel(ScreenMainLevel * mainlevel, double enemy_difficulty, double warp_fuel_bonus)
  : ScreenMainLevel(mainlevel->GetGameStats()),
    _mainlevel(mainlevel),
    _enemy_difficulty(enemy_difficulty),
    _warp_fuel_bonus(warp_fuel_bonus)
{
  auto myplr = _scene->GetPlayer();
  auto mainplr = _mainlevel->GetScene()->GetPlayer();
  myplr->CopyUpgrades(*mainplr);
  myplr->SetHealth(mainplr->GetHealth());
  myplr->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE)->SetEnabled(false);
  RefreshUI();
  _scene->EnableTutorialMessages(false);
}


void ScreenBonusLevel::SetupLevels()
{
  _levels.clear();
  auto level = new BonusLevel(_scene, _enemy_difficulty, _warp_fuel_bonus);
  _levels.push_back(level);
  OnLevelChanged();
}


void ScreenBonusLevel::OnQuit()
{
  auto myplr = _scene->GetPlayer();
  auto mainplr = _mainlevel->GetScene()->GetPlayer();
  mainplr->CopyUpgrades(*myplr);
  mainplr->SetHealth(myplr->GetHealth());
  mainplr->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE)->SetEnabled(true);
}
