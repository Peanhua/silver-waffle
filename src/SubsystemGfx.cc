/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemGfx.hh"
#include "GPUObject.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "Screen.hh"
#include "ShaderProgram.hh"
#include "SubsystemScreen.hh"
#include "SubsystemSettings.hh"
#include "UniformBufferObject.hh"
#include "Widget.hh"
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <thread>



SubsystemGfx * Graphics = nullptr;

#ifdef WITH_GPU_THREAD
thread_local unsigned int SubsystemGfx::current_buffer;
#endif


static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


SubsystemGfx::SubsystemGfx()
  : Subsystem("Gfx"),
    _window(nullptr)
#ifdef WITH_GPU_THREAD
  , _thread(nullptr)
#endif
{
}


bool SubsystemGfx::Start()
{
  bool rv = false;

  assert(!Graphics);
  Graphics = this;
  
  if(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)
    {
      auto screen_width = Settings->GetInt("screen_width");
      auto screen_height = Settings->GetInt("screen_height");
      _window = SDL_CreateWindow("Silver Waffle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

      if(_window)
        {
          SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
          SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
          SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
          SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
          
          _opengl_context = SDL_GL_CreateContext(_window);
          assert(_opengl_context);
          
          if(glewInit() == GLEW_OK)
            {
              if(GLEW_VERSION_4_1)
                {
                  glViewport(0, 0, screen_width, screen_height);
                  glClearColor(0, 0, 0, 0);
                  glEnable(GL_DEPTH_TEST);
                  glEnable(GL_CULL_FACE);
                  glCullFace(GL_BACK);
                  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                  
                  rv = true;
                  
                  ShaderProgram::current_program = 0;
                  UniformBufferObject::GetUniformBufferObject()->SetVec("in_resolution", glm::vec2(screen_width, screen_height));
#ifndef NDEBUG
                  if(GLEW_VERSION_4_3)
                    {
                      glEnable(GL_DEBUG_OUTPUT);
                      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                      glDebugMessageCallback(MessageCallback, 0);
                    }
#endif

#ifdef WITH_GPU_THREAD
                  SDL_GL_MakeCurrent(_window, nullptr);
                  current_buffer = 0;
                  _exit_thread = false;
                  _draw_frame = false;
                  _thread = new std::thread(Main, this);
                  assert(_thread->joinable());
#endif
                }
              else
                std::cerr << "Invalid OpenGL version." << std::endl;
            }
          else
            std::cerr << "Failed to initialize glew." << std::endl;
        }
      else
        std::cerr << "Failed to create window." << std::endl;
    }
  else
    std::cerr << "Failed to SDL_InitSubSystem(VIDEO):" << SDL_GetError() << std::endl;

  return rv;
}


void SubsystemGfx::Stop()
{
#ifdef WITH_GPU_THREAD
  if(_thread)
    {
      _exit_thread = true;
      _thread->join();
    }
  delete _thread;
  
  SDL_GL_MakeCurrent(_window, _opengl_context);
#endif

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  if(Graphics == this)
    Graphics = nullptr;
}


void SubsystemGfx::NextFrame()
{
#ifdef WITH_GPU_THREAD
  while(_draw_frame)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

  std::atomic_thread_fence(std::memory_order_release);

  _draw_frame = true;
  current_buffer ^= 1;
#else
  Draw();
#endif
}


#ifdef WITH_GPU_THREAD
void SubsystemGfx::Main(SubsystemGfx * gfx)
{
  SDL_GL_MakeCurrent(gfx->_window, gfx->_opengl_context);
  current_buffer = 0;
  while(true)
    {
      if(gfx->_exit_thread)
        break;

      if(gfx->_draw_frame)
        {
          gfx->Draw();
          gfx->_draw_frame = false;
          current_buffer ^= 1;
        }

      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  
  SDL_GL_MakeCurrent(gfx->_window, nullptr);
}
#endif


void SubsystemGfx::FlushQueues()
{
  if(_image_queue.size() > 0 || _mesh_vertex_queue.size() > 0)
    {
      std::lock_guard lock(_queue_mutex);
    
#ifdef WITH_GPU_THREAD
      for(auto shader : _shader_program_queue)
        shader->UpdateGPU();
      _shader_program_queue.clear();
#endif

      for(auto i : _image_queue)
        i->UpdateGPU();
      _image_queue.clear();

      for(auto p : _mesh_vertex_queue)
        p.first->UpdateGPU(Mesh::OPTION_VERTEX, p.second, 1);
      _mesh_vertex_queue.clear();
  
#ifdef WITH_GPU_THREAD
      for(auto m : _mesh_queue)
        if(m)
          m->UpdateGPU();
      _mesh_queue.clear();
  
      for(auto o : _object_queue)
        o->UpdateGPU();
      _object_queue.clear();
  
      for(auto w : _widget_queue)
        w->Render();
      _widget_queue.clear();
#endif
    }
}


void SubsystemGfx::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  FlushQueues();
  
  auto screen = ScreenManager->GetScreen();
  if(screen)
    screen->Draw();

  SDL_GL_SwapWindow(_window);
}  


static void GLAPIENTRY MessageCallback(GLenum source,
                                       GLenum type,
                                       GLuint id,
                                       GLenum severity,
                                       GLsizei length,
                                       const GLchar* message,
                                       const void* userParam)
{
  assert(source == source);
  assert(id == id);
  assert(length == length);
  assert(userParam == userParam);
  
  std::cerr << "OpenGL: " << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) << " type = " << type << ", severity = " << severity << ", message = " << message << std::endl;
  
  assert(type != GL_DEBUG_TYPE_ERROR);
}


void SubsystemGfx::QueueUpdateGPU(ShaderProgram * shader_program)
{
#ifdef WITH_GPU_THREAD
  std::lock_guard lock(_queue_mutex);
  _shader_program_queue.push_back(shader_program);
#else
  shader_program->UpdateGPU();
#endif
}


void SubsystemGfx::QueueUpdateGPU(Widget * widget)
{
#ifdef WITH_GPU_THREAD
  std::lock_guard lock(_queue_mutex);
  _widget_queue.push_back(widget);
#else
  widget->Render();
#endif
}


void SubsystemGfx::QueueUpdateGPU(Image * image)
{
  std::lock_guard lock(_queue_mutex);
  _image_queue.push_back(image);
}


void SubsystemGfx::QueueUpdateGPU(Mesh * mesh)
{
#ifdef WITH_GPU_THREAD
  std::lock_guard lock(_queue_mutex);
  _mesh_queue.push_back(mesh);
#else
  mesh->UpdateGPU();
#endif
}


void SubsystemGfx::QueueUpdateGPU(Mesh * mesh, unsigned int vertex_index)
{
  std::lock_guard lock(_queue_mutex);
  _mesh_vertex_queue.push_back({mesh, vertex_index});
}


void SubsystemGfx::QueueUpdateGPU(GPUObject * object)
{
#ifdef WITH_GPU_THREAD
  std::lock_guard lock(_queue_mutex);
  _object_queue.push_back(object);
#else
  object->UpdateGPU();
#endif
}

void SubsystemGfx::CancelUpdateGPU(Mesh * mesh)
{
#ifdef WITH_GPU_THREAD
  std::lock_guard lock(_queue_mutex);
  for(unsigned int i = 0; i < _mesh_queue.size(); i++)
    if(_mesh_queue[i] == mesh)
      _mesh_queue[i] = nullptr;
#else
  assert(mesh == mesh);
#endif
}
