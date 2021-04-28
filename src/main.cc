#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{ 
  int rv = EXIT_FAILURE;

  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
  
  if(SDL_Init(SDL_INIT_NOPARACHUTE) == 0)
    {
      if(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)
        {
          auto window = SDL_CreateWindow("Silver Waffle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

          if(window)
            {
              auto context = SDL_GL_CreateContext(window);

              if(glewInit() == GLEW_OK)
                {
                  if(GLEW_VERSION_3_0)
                    {
                      rv = EXIT_SUCCESS;
                      glClearColor(0, 0, 0, 1);
                      
                      while(true)
                        {
                          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                          
                          SDL_GL_SwapWindow(window);
                          double deltatime = 1.0 / 60.0;
                          double sleeptime = deltatime * 1000.0;
                          std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleeptime)));
                        }
                    }
                }
              else
                std::cerr << "Failed to initialize glew." << std::endl;
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
