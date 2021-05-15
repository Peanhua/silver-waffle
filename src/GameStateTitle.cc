#include "GameStateTitle.hh"
#include "GameStateGame.hh"
#include "Widget.hh"
#include "WidgetButton.hh"
#include <iostream>


GameStateTitle::GameStateTitle()
  : GameState(),
    _root(nullptr),
    _focused(nullptr)
{
  _root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(1024, 768));
  assert(_root);

  auto w = new WidgetButton(_root, glm::ivec2(1024/2 - 100, 768/2 - 100), glm::ivec2(200, 100));
  w->SetImage("Button-Play-Up");
  w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      SetChildState(new GameStateGame());
  });
  
  w = new WidgetButton(_root, glm::ivec2(1024/2 - 100, 768/2 + 100), glm::ivec2(200, 100));
  w->SetImage("Button-Quit-Up");
  w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      Quit();
  });
}


void GameStateTitle::Tick(double deltatime)
{
  assert(deltatime == deltatime);
  glDisable(GL_DEPTH_TEST);
  if(_root)
    _root->Draw();
}


void GameStateTitle::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

  if(pressed && key == SDLK_ESCAPE)
    Quit();
}


void GameStateTitle::OnMouseMove(const glm::ivec2 & position, const glm::ivec2 & relative_movement)
{
  assert(relative_movement == relative_movement);
  
  if(!_root)
    return;

  auto focused = _root->GetWidgetAt(position);
  if(focused)
    {
      if(focused != _focused)
        {
          if(_focused)
            _focused->SetIsFocused(false);
          _focused = focused;
          _focused->SetIsFocused(true);
        }
    }
  else
    {
      if(_focused)
        _focused->SetIsFocused(false);
      _focused = nullptr;
    }
}


void GameStateTitle::OnMouseButton(bool pressed, unsigned int button, const glm::ivec2 & position)
{
  assert(button == button);
  
  if(_root)
    {
      auto w = _root->GetWidgetAt(position);
      if(w)
        w->OnClicked(pressed);
    }
}
