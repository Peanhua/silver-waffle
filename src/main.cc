#include "SubsystemHandler.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGameState.hh"
#include "SubsystemGfx.hh"
#include "SubsystemInput.hh"
#include <SDL.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>


int main(int argc, char *argv[])
{
  assert(argc == argc);
  assert(argv == argv);
  
  int rv = EXIT_FAILURE;

  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
  
  if(SDL_Init(SDL_INIT_NOPARACHUTE) == 0)
    {
      SubsystemHandler ssh;

      {
        SubsystemAssetLoader ass;
        SubsystemGfx gfx;
        SubsystemGameState gs;
        SubsystemInput inp(gs);
        ssh.Add(&ass);
        ssh.Add(&inp);
        ssh.Add(&gfx);
        ssh.Add(&gs);

        if(ssh.StartAll())
          {
            rv = EXIT_SUCCESS;
            
            while(ssh.IsRunning())
              {
                double deltatime = 1.0 / 60.0;
                ssh.PreTickAll();
                ssh.TickAll(deltatime);
                ssh.PostTickAll();
                
                double sleeptime = deltatime * 1000.0;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleeptime)));
              }
          }
        else
          std::cerr << "Failed to start subsystems." << std::endl;
        
        ssh.StopAll();
      }
      
      SDL_Quit();
    }
  else
    std::cerr << "Failed to SDL_Init():" << SDL_GetError() << std::endl;
  
  return rv;
}
