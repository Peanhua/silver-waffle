/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemHandler.hh"
#include "Camera.hh"
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
  bool demo = false;
  for(int i = 1; i < argc; i++)
    if(std::string(argv[i]) == "--fps")
      unlimited_fps = true;
    else if(std::string(argv[i]) == "--demo")
      demo = true;
    else
      std::cerr << argv[0] << ": Warning, unknown argument #" << i << "'" << argv[i] << "'\n";

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

            if(unlimited_fps || demo)
              SDL_GL_SetSwapInterval(0);
            Settings->SetBool("demo", demo);

            ass.LoadCache();
            
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
                gfx.NextFrame();
                auto frame_end = clock.now();

                auto frame_time = std::chrono::duration<double>(frame_end - frame_start);
                if(unlimited_fps)
                  {
                    deltatime = frame_time;
                  }
                else if(!demo)
                  {
                    auto sleeptime = deltatime - frame_time;
                    if(sleeptime.count() > 0.0)
                      std::this_thread::sleep_for(sleeptime);
                  }

                framecounter++;

                auto since_last_fps = std::chrono::duration<double>(frame_end - last_fps_time);
                if(since_last_fps.count() > 5.0 || !ssh.IsRunning())
                  {
                    std::cout << "frames: " << framecounter << ", fps: " << (static_cast<double>(framecounter) / since_last_fps.count());
                    framecounter = 0;
                    last_fps_time = frame_end;

                    auto s = dynamic_cast<ScreenLevel *>(ScreenManager->GetScreen());
                    if(s)
                      {
                        auto scene = s->GetScene();
                        assert(scene);

                        {
                          auto stats = scene->GetCollisionCheckStatistics();
                          std::cout << "    "
                                    << "collisions:"
                                    << " elapsed time=" << stats.elapsed_time
                                    << ", objects total=" << stats.total << "("
                                    << std::floor(static_cast<double>(stats.total) / stats.elapsed_time) << "/s, "
                                    << std::floor(static_cast<double>(stats.total) / static_cast<double>(stats.elapsed_frames)) << "/frame"
                                    << ")"
                                    << ", passed wide check=" << stats.pass_wide_check << "("
                                    << (100.0 * static_cast<double>(stats.pass_wide_check) / static_cast<double>(stats.total)) << "%, "
                                    << std::floor(static_cast<double>(stats.pass_wide_check) / static_cast<double>(stats.elapsed_frames)) << "/frame"
                                    << ")"
                                    << ", passed narrow check=" << stats.pass_narrow_check << "(" << (100.0 * static_cast<double>(stats.pass_narrow_check) / static_cast<double>(stats.total)) << "%)";
                          scene->ResetCollisionCheckStatistics();
                        }
                        {
                          auto & stats = s->GetCamera()->stats;
                          std::cout << "    "
                                    << "view frustum culling:"
                                    << " total=" << (stats.in_view + stats.out_view)
                                    << " inside=" << stats.in_view
                                    << "(" << (100.0 * static_cast<double>(stats.in_view) / static_cast<double>(stats.in_view + stats.out_view)) << "%, "
                                    << std::floor(static_cast<double>(stats.in_view) / static_cast<double>(stats.elapsed_frames)) << "/frame"
                                    << ")"
                                    << " outside=" << stats.out_view
                                    << "(" << (100.0 * static_cast<double>(stats.out_view) / static_cast<double>(stats.in_view + stats.out_view)) << "%, "
                                    << std::floor(static_cast<double>(stats.out_view) / static_cast<double>(stats.elapsed_frames)) << "/frame"
                                    << ")";
                          stats.elapsed_frames = 0;
                          stats.in_view = 0;
                          stats.out_view = 0;
                        }
                      }
                    std::cout << std::endl;

                    if(s)
                      s->GetScene()->DumpStats();
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
