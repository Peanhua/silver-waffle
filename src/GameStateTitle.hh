#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_

#include "GameState.hh"

class Starfield;


class GameStateTitle : public GameState
{
public:
  GameStateTitle();

  void Tick(double deltatime) override;
  
private:
  Starfield * _starfield;
};

#endif
