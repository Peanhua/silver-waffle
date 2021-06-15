#ifndef GAME_STATS_HH_
#define GAME_STATS_HH_

#include "UpgradeMaterial.hh"
#include <vector>


class GameStats
{
public:
  GameStats();
  
  void         Tick(double deltatime);

  int          GetLives() const;
  void         AdjustLives(int amount);
  
  unsigned int GetScore() const;
  void         AddScore(unsigned int amount);
  unsigned int GetScoreMultiplier() const;
  void         SetScoreMultiplier(unsigned int multiplier, double timer);

  UpgradeMaterial * GetUpgradeMaterial(UpgradeMaterial::Type type);
  std::vector<UpgradeMaterial *> & GetUpgradeMaterials();

private:
  int _lives;
  
  unsigned int _score;
  unsigned int _score_multiplier;
  double       _score_multiplier_timer;

  std::vector<UpgradeMaterial *> _upgradematerials;
};

#endif
