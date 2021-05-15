#ifndef SUBSYSTEM_GAME_STATE_HH_
#define SUBSYSTEM_GAME_STATE_HH_

#include "Subsystem.hh"
#include <vector>

class GameState;

class SubsystemGameState : public Subsystem
{
public:
  SubsystemGameState();
  
  bool Start() override;
  void Stop() override;
  void Tick(double deltatime) override;
  bool IsRunning() const override;

  GameState * GetGameState() const;
  
private:
  GameState * _root_gamestate;
};

#endif

