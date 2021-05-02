#include "SubsystemHandler.hh"
#include <cassert>
#include <iostream>


bool SubsystemHandler::StartAll()
{
  bool rv = true;
  
  for(auto it = _subsystems.begin(); rv == true && it != _subsystems.end(); it++)
    {
      assert(*it);
      std::cout << "Starting " << (*it)->GetName() << std::endl;
      rv = (*it)->Start();
    }
  
  return rv;
}


void SubsystemHandler::StopAll()
{
  for(auto it = _subsystems.rbegin(); it != _subsystems.rend(); it++)
    (*it)->Stop();
}


void SubsystemHandler::Add(Subsystem * subsystem)
{
  assert(subsystem);
  _subsystems.push_back(subsystem);
}


void SubsystemHandler::PreTickAll()
{
  for(auto s : _subsystems)
    s->PreTick();
}


void SubsystemHandler::TickAll(double deltatime)
{
  for(auto s : _subsystems)
    s->Tick(deltatime);
}


void SubsystemHandler::PostTickAll()
{
  for(auto s : _subsystems)
    s->PostTick();
}


bool SubsystemHandler::IsRunning() const
{
  bool rv = true;
  for(auto s : _subsystems)
    if(!s->IsRunning())
      rv = false;
  return rv;
}
