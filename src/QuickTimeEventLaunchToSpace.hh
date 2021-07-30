#ifndef QUICK_TIME_EVENT_LAUNCH_TO_SPACE_HH_
#define QUICK_TIME_EVENT_LAUNCH_TO_SPACE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "SubsystemAssetLoader.hh"
#include <SDL_keyboard.h>

class ObjectSpaceship;
class Widget;


class QuickTimeEvent
{
public:
  QuickTimeEvent(ObjectSpaceship * player, Widget * root_widget, double time);
  virtual ~QuickTimeEvent();
  
  void Tick(double deltatime);
  bool IsAlive() const;
  virtual bool OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) = 0;

protected:
  ObjectSpaceship * _player;
  double            _timer;
  bool              _quit;
  std::string       _message;

private:
  std::string _last_message;
  Widget *    _message_widget;
};


class QuickTimeEventLaunchToSpace : public QuickTimeEvent
{
public:
  QuickTimeEventLaunchToSpace(ObjectSpaceship * player, Widget * root_widget, double time);
  bool OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;
};


#endif
