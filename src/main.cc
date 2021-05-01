#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <SDL_keyboard.h>

#include "GameLevel.hh"
#include "OpenGL.hh"
#include "ShaderProgram.hh"
#include "Camera.hh"
#include "InputManager.hh"


int main(int argc, char *argv[])
{
  assert(argc == argc);
  assert(argv == argv);
  
  int rv = EXIT_FAILURE;

  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
  
  if(SDL_Init(SDL_INIT_NOPARACHUTE) == 0)
    {
      if(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)
        {
          auto window = SDL_CreateWindow("Silver Waffle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

          if(window)
            {
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

              auto context = SDL_GL_CreateContext(window);
              assert(context);

              if(glewInit() == GLEW_OK)
                {
                  if(GLEW_VERSION_3_2)
                    {
                      rv = EXIT_SUCCESS;

                      OpenGL::singleton = new OpenGL::Singleton();

                      glViewport(0, 0, 1024, 768);


                      auto inp = new InputManager();
                      assert(inp);

                      GLuint vao;
                      glGenVertexArrays(1, &vao);
                      glBindVertexArray(vao);
                      
                      std::string vs(R"(
#version 330 core

uniform mat4 in_mvp;

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 in_color;

out vec3 diffuse_color;

void main()
{
  diffuse_color = in_color;
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
)");

                      std::string fs(R"(
#version 330 core

in vec3 diffuse_color;
out vec4 final_colour;

void main()
{
  final_colour = vec4(diffuse_color, 1);
}
)");

                      std::vector<std::string> vss({vs});
                      std::vector<std::string> fss({fs});

                      auto p = new ShaderProgram(vss, fss);
                      glUseProgram(p->GetProgram());

                      auto camera = new Camera();
                      camera->SetPosition(glm::dvec3(0, -3, 0));
                      camera->SetTargetPosition(glm::dvec3(0, 0, 0));

                      glClearColor(0, 0, 0, 1);
                      glEnable(GL_DEPTH_TEST);
                      glDisable(GL_CULL_FACE);
                      //glCullFace(GL_BACK);
                      
                      glm::vec3 pos(0, 0, 0);
                      auto level = new GameLevel(pos);
                      level->Initialize(1.0);

                      double fov = 60.0;
                      
                      inp->SetOnKeyboard([camera, &fov](bool pressed, SDL_Keycode key, SDL_Keymod mod)
                      {
                        if(!pressed)
                          return;

                        double speed = 0.5;
                        switch(key)
                          {
                          case SDLK_LEFT:
                            camera->MoveRight(-speed);
                            break;
                          case SDLK_RIGHT:
                            camera->MoveRight(speed);
                            break;
                          case SDLK_UP:
                            camera->MoveForward(speed);
                            break;
                          case SDLK_DOWN:
                            camera->MoveForward(-speed);
                            break;
                          case SDLK_PAGEUP:
                            camera->MoveUp(speed);
                            break;
                          case SDLK_PAGEDOWN:
                            camera->MoveUp(-speed);
                            break;
                          case SDLK_PLUS:
                            fov++;
                            std::cout << "fov:" << fov << std::endl;
                            camera->SetFOV(fov);
                            break;
                          case SDLK_MINUS:
                            fov--;
                            std::cout << "fov:" << fov << std::endl;
                            camera->SetFOV(fov);
                            break;
                          }
                      });

                      
                      while(inp->IsRunning())
                        {
                          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                          level->Draw(*camera, *p);
                          SDL_GL_SwapWindow(window);
                          
                          double deltatime = 1.0 / 60.0;
                          double sleeptime = deltatime * 1000.0;
                          std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleeptime)));
                          inp->Tick(deltatime);
                          level->Tick(deltatime);
                        }

                      delete level;
                    }
                }
              else
                std::cerr << "Failed to initialize glew." << std::endl;

              delete OpenGL::singleton;
            }
          else
            std::cerr << "Failed to create window." << std::endl;
        }
      else
        std::cerr << "Failed to SDL_InitSubSystem(VIDEO):" << SDL_GetError() << std::endl;
      
      SDL_Quit();
    }
  else
    std::cerr << "Failed to SDL_Init():" << SDL_GetError() << std::endl;
  
  return rv;
}
