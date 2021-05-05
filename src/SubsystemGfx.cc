#include "SubsystemGfx.hh"
#include "ShaderProgram.hh"
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
      _window = SDL_CreateWindow("Silver Waffle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

      if(_window)
        {
          SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
          SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
          SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
          
          auto context = SDL_GL_CreateContext(_window);
          assert(context);
          
          if(glewInit() == GLEW_OK)
            {
              if(GLEW_VERSION_3_2)
                {
                  glViewport(0, 0, 1024, 768);
                  glClearColor(0, 0, 0, 1);
                  glEnable(GL_DEPTH_TEST);
                  glDisable(GL_CULL_FACE);
                  //glCullFace(GL_BACK);

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
