/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "QuickTimeEventLaunchToSpace.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemSettings.hh"
#include "Widget.hh"


QuickTimeEvent::QuickTimeEvent(ObjectSpaceship * player, Widget * root_widget, double time)
  : _player(player),
    _timer(time),
    _quit(false)
{
  auto w = Settings->GetInt("screen_width");
  auto h = Settings->GetInt("screen_height");
  _message_widget = new Widget(root_widget, glm::ivec2(0, 0), glm::ivec2(w, h));
  _message_widget->SetTextFont(AssetLoader->LoadFont(30));
  _message_widget->SetTextColor(glm::vec3(1, 1, 1));
}


QuickTimeEvent::~QuickTimeEvent()
{
  assert(_message_widget);
  _message_widget->Destroy();
}


void QuickTimeEvent::Tick(double deltatime)
{
  if(!IsAlive())
    return;
  
  _timer -= deltatime;
  if(_timer < 0)
    return;

  auto fmt = [](double v)
  {
    auto rv = std::to_string(v);
    return rv.substr(0, rv.find('.') + 3);
  };
  auto m = _message + fmt(_timer);
  if(m != _last_message)
    {
      _last_message = m;
      _message_widget->SetText(m);
      _message_widget->SetTextPaddingCentered(true, true);
    }
}


bool QuickTimeEvent::IsAlive() const
{
  if(_quit)
    return false;
  if(_timer < 0)
    return false;
  return true;
}


QuickTimeEventLaunchToSpace::QuickTimeEventLaunchToSpace(ObjectSpaceship * player, Widget * root_widget, double time)
  : QuickTimeEvent(player, root_widget, time)
{
  player->SystemlogAppend("Preparing for launch...\n");
  _message = "Press 'L' to launch into space in ";
}


bool QuickTimeEventLaunchToSpace::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

  if(_quit)
    return false;
  
  if(key == SDLK_l)
    {
      if(pressed)
        {
          if(_timer > 0 && _timer < 0.5)
            {
              _player->LaunchToSpace();
            }
          else
            {
              _player->LaunchFromSpaceport();
              _player->SystemlogAppend("Launch failed: Timing error\n");
            }
          _quit = true;
        }
      return true;
    }

  if(pressed)
    _quit = true;
  return false;
}
