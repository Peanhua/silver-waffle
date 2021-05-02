#include "SubsystemGameState.hh"
#include "GameStateGame.hh"

SubsystemGameState::SubsystemGameState()
  : Subsystem("GameState"),
    _gamestate(nullptr)
{
}


bool SubsystemGameState::Start()
{
  _gamestate = new GameStateGame();
  return _gamestate;
}


void SubsystemGameState::Stop()
{
  _gamestate = nullptr;
}


void SubsystemGameState::Tick(double deltatime)
{
  if(!_gamestate)
    return;

  _gamestate->Tick(deltatime);
  _gamestate = _gamestate->GetCurrentState();
}


bool SubsystemGameState::IsRunning() const
{
  return _gamestate;
}


GameState * SubsystemGameState::GetGameState() const
{
  return _gamestate;
}

