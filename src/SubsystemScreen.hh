#ifndef SUBSYSTEM_GAME_STATE_HH_
#define SUBSYSTEM_GAME_STATE_HH_

#include "Subsystem.hh"
#include <vector>

class Screen;

class SubsystemScreen : public Subsystem
{
public:
  SubsystemScreen();
  
  bool Start() override;
  void Stop() override;
  void Tick(double deltatime) override;
  bool IsRunning() const override;

  Screen * GetScreen() const;
  
private:
  Screen * _root_screen;
};

extern SubsystemScreen * ScreenManager;

#endif

