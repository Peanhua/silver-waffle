#include "SubsystemHandler.hh"
#include "Scene.hh"
#include "ScreenLevel.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include "SubsystemHighscores.hh"
#include "SubsystemInput.hh"
#include "SubsystemSettings.hh"
#include <SDL.h>
#include <iostream>
#include <chrono>
#include <thread>


int main(int argc, char *argv[])
{
  bool unlimited_fps = false;
  if(argc > 1)
    if(std::string(argv[1]) == "--fps")
      unlimited_fps = true;
  
  int rv = EXIT_FAILURE;

  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
  
  if(SDL_Init(SDL_INIT_NOPARACHUTE) == 0)
    {
      SubsystemHandler ssh;

      {
        SubsystemAssetLoader ass;
        SubsystemSettings    settings;
        SubsystemHighscores  highscores;
        SubsystemGfx         gfx;
        SubsystemScreen      screen;
        SubsystemInput       inp;
        ssh.Add(&ass);
        ssh.Add(&settings);
        ssh.Add(&highscores);
        ssh.Add(&inp);
        ssh.Add(&gfx);
        ssh.Add(&screen);

        if(ssh.StartAll())
          {
            rv = EXIT_SUCCESS;

            if(unlimited_fps)
              SDL_GL_SetSwapInterval(0);
            
            std::chrono::steady_clock clock;
            std::chrono::duration<double> deltatime(1.0 / 60.0);

            auto last_fps_time = clock.now();
            unsigned int framecounter = 0;
            
            while(ssh.IsRunning())
              {
                auto frame_start = clock.now();
                ssh.PreTickAll();
                ssh.TickAll(deltatime.count());
                ssh.PostTickAll();
                auto frame_end = clock.now();

                auto frame_time = std::chrono::duration<double>(frame_end - frame_start);
                if(unlimited_fps)
                  {
                    deltatime = frame_time;
                  }
                else
                  {
                    auto sleeptime = deltatime - frame_time;
                    if(sleeptime.count() > 0.0)
                      std::this_thread::sleep_for(sleeptime);
                  }

                framecounter++;

                auto since_last_fps = std::chrono::duration<double>(frame_end - last_fps_time);
                if(since_last_fps.count() > 5.0)
                  {
                    std::cout << "fps: " << (static_cast<double>(framecounter) / since_last_fps.count());
                    framecounter = 0;
                    last_fps_time = frame_end;

                    auto s = dynamic_cast<ScreenLevel *>(ScreenManager->GetScreen());
                    if(s)
                      {
                        auto scene = s->GetScene();
                        assert(scene);

                        auto stats = scene->GetCollisionCheckStatistics();
                        std::cout << "    "
                                  << "collisions:"
                                  << " elapsed time=" << stats.elapsed_time
                                  << ", objects total=" << stats.total << "(" << std::floor(static_cast<double>(stats.total) / stats.elapsed_time) << "/s)"
                                  << ", passed wide check=" << stats.pass_wide_check << "(" << (100.0 * static_cast<double>(stats.pass_wide_check) / static_cast<double>(stats.total)) << "%)"
                                  << ", passed narrow check=" << stats.pass_narrow_check << "(" << (100.0 * static_cast<double>(stats.pass_narrow_check) / static_cast<double>(stats.total)) << "%)";
                        scene->ResetCollisionCheckStatistics();
                      }
                    std::cout << std::endl;
                  }
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
