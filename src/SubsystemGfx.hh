#ifndef SUBSYSTEM_GFX_HH_
#define SUBSYSTEM_GFX_HH_

#include "Subsystem.hh"
#include <SDL.h>

class SubsystemGfx : public Subsystem
{
public:
  SubsystemGfx();
  
  bool Start() override;
  void Stop() override;
  void PreTick() override;
  void PostTick() override;

private:
  SDL_Window * _window;
};

#endif
