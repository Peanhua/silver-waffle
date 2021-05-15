#include "GameState.hh"
#include "Widget.hh"
#include <cassert>
#include <GL/glew.h>


GameState::GameState()
  : _running(true),
    _child(nullptr),
    _root_widget(nullptr),
    _focused_widget(nullptr)
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
  glDisable(GL_DEPTH_TEST);
  if(_root_widget)
    _root_widget->Draw();
}


void GameState::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

  if(pressed && key == SDLK_ESCAPE)
    Quit();
}


void GameState::OnMouseMove(const glm::ivec2 & position, const glm::ivec2 & relative_movement)
{
  assert(relative_movement == relative_movement);
  
  if(!_root_widget)
    return;

  auto focused = _root_widget->GetWidgetAt(position);
  if(focused)
    {
      if(focused != _focused_widget)
        {
          if(_focused_widget)
            _focused_widget->SetIsFocused(false);
          _focused_widget = focused;
          _focused_widget->SetIsFocused(true);
        }
    }
  else
    {
      if(_focused_widget)
        _focused_widget->SetIsFocused(false);
      _focused_widget = nullptr;
    }
}


void GameState::OnMouseButton(bool pressed, unsigned int button, const glm::ivec2 & position)
{
  assert(button == button);
  
  if(_root_widget)
    {
      auto w = _root_widget->GetWidgetAt(position);
      if(w)
        w->OnClicked(pressed);
    }
}


void GameState::SetRootWidget(Widget * widget)
{
  if(widget)
    assert(!_root_widget);
  else
    assert(_root_widget);
  
  _root_widget = widget;
}


Widget * GameState::GetRootWidget() const
{
  return _root_widget;
}
