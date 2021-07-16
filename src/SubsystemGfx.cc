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
#include "ShaderProgram.hh"
#include "SubsystemSettings.hh"
#include <GL/glew.h>
#include <SDL.h>
#include <iostream>


static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);



SubsystemGfx::SubsystemGfx()
  : Subsystem("Gfx"),
    _window(nullptr)
{
}


bool SubsystemGfx::Start()
{
  bool rv = false;
  
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
          
          auto context = SDL_GL_CreateContext(_window);
          assert(context);
          
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
                  

#ifndef NDEBUG
                  if(GLEW_VERSION_4_3)
                    {
                      glEnable(GL_DEBUG_OUTPUT);
                      glDebugMessageCallback(MessageCallback, 0);
                    }
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
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


void SubsystemGfx::PreTick()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void SubsystemGfx::PostTick()
{
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
