#ifndef GAME_STATE_BONUS_LEVEL_HH_
#define GAME_STATE_BONUS_LEVEL_HH_

#include "GameStateGame.hh"


class GameStateBonusLevel : public GameStateGame
{
public:
  GameStateBonusLevel(GameStateGame * mainlevel, double enemy_difficulty, double warp_fuel_bonus);
  
  void SetupLevels() override;

private:
  GameStateGame * _mainlevel;
  double _enemy_difficulty;
  double _warp_fuel_bonus;

  void OnQuit() override;
};

#endif
