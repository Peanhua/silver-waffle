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

class GPUObject;
class Image;
class Mesh;
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

  void QueueUpdateGPU(GPUObject * object);
  void QueueUpdateGPU(ShaderProgram * shader_program);
  void QueueUpdateGPU(Widget * widget);
  void QueueUpdateGPU(Image * image);
  void QueueUpdateGPU(Mesh * mesh);
  void QueueUpdateGPU(Mesh * mesh, unsigned int vertex_index);

  void CancelUpdateGPU(Mesh * mesh);

private:
  SDL_Window *  _window;
  SDL_GLContext _opengl_context;

  std::vector<GPUObject *>     _object_queue;
  std::vector<ShaderProgram *> _shader_program_queue;
  std::vector<Widget *>        _widget_queue;
  std::vector<Image *>         _image_queue;
  std::vector<Mesh *>          _mesh_queue;
  std::vector<std::pair<Mesh *, unsigned int>> _mesh_vertex_queue;
};


extern SubsystemGfx * Graphics;

#endif
