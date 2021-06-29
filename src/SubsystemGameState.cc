#include "SubsystemGameState.hh"
#include "GameStateTitle.hh"


SubsystemGameState * GameStateManager = nullptr;


SubsystemGameState::SubsystemGameState()
  : Subsystem("GameState"),
    _root_gamestate(nullptr)
{
}


bool SubsystemGameState::Start()
{
  assert(!GameStateManager);
  GameStateManager = this;
  
  _root_gamestate = new GameStateTitle();
  return GetGameState();
}


void SubsystemGameState::Stop()
{
  while(auto state = GetGameState())
    state->Quit();
  
  if(GameStateManager == this)
    GameStateManager = nullptr;

  delete _root_gamestate;
  _root_gamestate = nullptr;
}


void SubsystemGameState::Tick(double deltatime)
{
  auto state = GetGameState();
  if(!state)
    return;

  auto child = state->GetChildState();
  if(child && !child->IsRunning())
    {
      state->SetChildState(nullptr);
      delete child;
    }

  state->Tick(deltatime);
}


bool SubsystemGameState::IsRunning() const
{
  auto state = GetGameState();
  if(!state)
    return false;

  return state->IsRunning();
}


GameState * SubsystemGameState::GetGameState() const
{
  auto rv = _root_gamestate;
  if(rv && !rv->IsRunning())
    return nullptr;

  auto next = rv;
  while(next && next->IsRunning())
    {
      rv = next;
      next = rv->GetChildState();
    }

  return rv;
}

