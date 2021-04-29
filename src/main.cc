#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

#include "GameLevel.hh"
#include "OpenGL.hh"
#include "ShaderProgram.hh"
#include "Camera.hh"

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
              /*
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
              */
              auto context = SDL_GL_CreateContext(window);
              assert(context);

              if(glewInit() == GLEW_OK)
                {
                  if(GLEW_VERSION_3_0)
                    {
                      rv = EXIT_SUCCESS;

                      OpenGL::singleton = new OpenGL::Singleton();


                      std::string vs(R"(
#version 130
#extension GL_ARB_explicit_attrib_location : require

in vec3 in_vertex;
in vec3 in_color;
uniform mat4 in_mvp;

out vec3 diffuse_color;

void main()
{
  diffuse_color = in_color;

  //gl_Position = ftransform();
  //gl_Position = gl_ModelViewProjectionMatrix * vec4(in_vertex, 1.0f);
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
)");

                      std::string fs(R"(
#version 130

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
                      camera->SetPosition(glm::dvec3(1, 1, 1));
                      camera->SetTargetPosition(glm::dvec3(0, 0, 0));
                      
                      glClearColor(0, 0, 0, 1);

                      glm::vec3 pos(0, 0, 0);
                      auto level = new GameLevel(pos);
                      level->Initialize(1.0);
                      
                      while(true)
                        {
                          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                          level->Draw(*camera, *p);
                          SDL_GL_SwapWindow(window);
                          
                          double deltatime = 1.0 / 60.0;
                          double sleeptime = deltatime * 1000.0;
                          std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleeptime)));
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
