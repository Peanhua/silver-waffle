#include "GameState.hh"
#include <cassert>


GameState::GameState()
  : _running(true),
    _child(nullptr)
{
}


GameState::~GameState()
{
}


GameState * GameState::GetChildState() const
{
  return _child;
}


void GameState::SetChildState(GameState * gamestate)
{
  if(gamestate)
    assert(!_child);
  else
    assert(_child);
  
  _child = gamestate;
}


bool GameState::IsRunning() const
{
  return _running;
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


void GameState::OnMouseMove(const glm::ivec2 & position, const glm::ivec2 & relative_movement)
{
  assert(position == position);
  assert(relative_movement == relative_movement);
}


void GameState::OnMouseButton(bool pressed, unsigned int button, const glm::ivec2 & position)
{
  assert(pressed == pressed);
  assert(button == button);
  assert(position == position);
}

