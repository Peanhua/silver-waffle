#ifndef SUBSYSTEM_INPUT_HH_
#define SUBSYSTEM_INPUT_HH_

#include "Subsystem.hh"
#include "SubsystemGameState.hh"
#include <SDL_keyboard.h>

class SubsystemInput : public Subsystem
{
public:
  SubsystemInput(SubsystemGameState & subsystem_gamestate);

  bool Start() override;
  void Stop() override;
  void Tick(double deltatime) override;
  
  bool IsRunning() const override;

  
private:
  bool                 _running;
  SubsystemGameState & _subsystem_gamestate;
};

#endif
