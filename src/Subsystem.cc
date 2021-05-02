#include "Subsystem.hh"
#include <cassert>

Subsystem::Subsystem(const std::string & name)
  : _name(name)
{
}


Subsystem::~Subsystem()
{
}


const std::string & Subsystem::GetName() const
{
  return _name;
}


void Subsystem::PreTick()
{
}


void Subsystem::Tick(double deltatime)
{
  assert(deltatime == deltatime);
}


void Subsystem::PostTick()
{
}


bool Subsystem::IsRunning() const
{
  return true;
}



