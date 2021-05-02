#include "GameState.hh"
#include <cassert>


GameState::GameState()
  : _running(true)
{
}


GameState::~GameState()
{
}


GameState * GameState::GetCurrentState()
{
  if(_running)
    return this;
  else
    return nullptr;
}


void GameState::Quit()
{
  _running = false;
}


void GameState::Tick(double deltatime)
{
  assert(deltatime == deltatime);
}


void GameState::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(pressed == pressed);
  assert(key == key);
  assert(mod == mod);
}
