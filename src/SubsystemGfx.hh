#ifndef SUBSYSTEM_GFX_HH_
#define SUBSYSTEM_GFX_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Subsystem.hh"
#include <SDL.h>
#include <vector>

class ShaderProgram;
class Widget;


class SubsystemGfx : public Subsystem
{
public:
  SubsystemGfx();
  
  bool Start()    override;
  void Stop()     override;
  void PreTick()  override;
  void Tick(double deltatime) override;
  void PostTick() override;

  void QueueUpdateGPU(ShaderProgram * shader_program);
  void QueueUpdateGPU(Widget * widget);

private:
  SDL_Window * _window;
  std::vector<ShaderProgram *> _shader_program_queue;
  std::vector<Widget *>        _widget_queue;
};


extern SubsystemGfx * Graphics;

#endif
