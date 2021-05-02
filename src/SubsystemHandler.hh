#ifndef SUBSYSTEM_HANDLER_HH_
#define SUBSYSTEM_HANDLER_HH_

#include "Subsystem.hh"
#include <vector>

class SubsystemHandler
{
public:
  bool StartAll();
  void StopAll();
  void Add(Subsystem * subsystem);
  void PreTickAll();
  void TickAll(double deltatime);
  void PostTickAll();
  bool IsRunning() const;

private:
  std::vector<Subsystem *> _subsystems;
};

#endif
