#include "GameStats.hh"
#include "SubsystemSettings.hh"
#include <cassert>


GameStats::GameStats()
  : _lives(3),
    _score(0),
    _score_multiplier(1),
    _score_multiplier_timer(0)
{
  _upgradematerials.push_back(new UpgradeMaterial(UpgradeMaterial::Type::ATTACK,   "Material A"));
  _upgradematerials.push_back(new UpgradeMaterial(UpgradeMaterial::Type::DEFENSE,  "Material D"));
  _upgradematerials.push_back(new UpgradeMaterial(UpgradeMaterial::Type::PHYSICAL, "Material P"));
  if(Settings->GetBool("cheat_cheap_upgrades"))
    for(auto u : _upgradematerials)
      u->Add(9999);
}


void GameStats::Tick(double deltatime)
{
  if(_score_multiplier_timer > 0.0)
    _score_multiplier_timer -= deltatime;
}


void GameStats::AddScore(unsigned int amount)
{
  if(_score_multiplier_timer > 0.0)
    amount *= _score_multiplier;

  _score += amount;
}


void GameStats::SetScoreMultiplier(unsigned int multiplier, double timer)
{
  _score_multiplier = multiplier;
  _score_multiplier_timer = timer;
}


unsigned int GameStats::GetScore() const
{
  return _score;
}


unsigned int GameStats::GetScoreMultiplier() const
{
  if(_score_multiplier_timer > 0.0)
    return _score_multiplier;
  return 1;
}


UpgradeMaterial * GameStats::GetUpgradeMaterial(UpgradeMaterial::Type type)
{
  for(auto u : _upgradematerials)
    if(u->GetType() == type)
      return u;
  
  assert(false);
  return nullptr;
}


std::vector<UpgradeMaterial *> & GameStats::GetUpgradeMaterials()
{
  return _upgradematerials;
}


int GameStats::GetLives() const
{
  return _lives;
}


void GameStats::AdjustLives(int amount)
{
  _lives += amount;
}

