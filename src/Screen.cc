/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Screen.hh"
#include "Widget.hh"
#include <cassert>
#include <GL/glew.h>


Screen::Screen()
  : _running(true),
    _child(nullptr),
    _root_widget(nullptr),
    _modal_widget(nullptr)
{
}


Screen::~Screen()
{
  delete _child;
  delete _root_widget;
}


Screen * Screen::GetChild() const
{
  return _child;
}


void Screen::SetChild(Screen * screen)
{
  if(screen)
    assert(!_child);
  else
    assert(_child);
  
  _child = screen;
}


bool Screen::IsRunning() const
{
  return _running;
}


void Screen::Quit()
{
  _running = false;
  OnQuit();
}


void Screen::Tick(double deltatime)
{
  if(_root_widget)
    _root_widget->Tick(deltatime);
}


void Screen::Draw() const
{
  if(_root_widget)
    {
      glDisable(GL_DEPTH_TEST);
      _root_widget->Draw();
    }
}


void Screen::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

  if(pressed && key == SDLK_ESCAPE)
    Quit();
}


void Screen::OnMouseMove(const glm::ivec2 & position, const glm::ivec2 & relative_movement)
{
  assert(relative_movement == relative_movement);
  
  if(!_root_widget)
    return;

  auto start = _root_widget;
  if(_modal_widget)
    start = _modal_widget;
  
  auto current = _root_widget->GetFocusedWidget();
  auto focused = start->GetWidgetAt(position);
  if(focused)
    {
      if(focused->GetIsFocusable())
        if(focused != current)
          {
            if(current)
              current->SetIsFocused(false);
            focused->SetIsFocused(true);
          }
    }
  else
    {
      if(current)
        current->SetIsFocused(false);
    }
}


void Screen::OnMouseButton(bool pressed, unsigned int button, const glm::ivec2 & position)
{
  assert(button == button);
  
  if(_root_widget)
    {
      auto start = _root_widget;
      if(_modal_widget)
        start = _modal_widget;
      
      auto w = start->GetWidgetAt(position);
      if(w)
        w->OnClicked(pressed);
    }
}


void Screen::SetRootWidget(Widget * widget)
{
  if(widget)
    assert(!_root_widget);
  else
    assert(_root_widget);
  
  _root_widget = widget;
}


Widget * Screen::GetRootWidget() const
{
  return _root_widget;
}


void Screen::SetModalWidget(Widget * widget)
{
  _modal_widget = widget;
}


void Screen::OnQuit()
{
}
