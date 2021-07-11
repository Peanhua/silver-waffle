#include "SubsystemSettings.hh"
#include <cassert>

SubsystemSettings * Settings = nullptr;


SubsystemSettings::SubsystemSettings()
  : Subsystem("Settings")
{
}


bool SubsystemSettings::Start()
{
  if(Settings)
    return false;
  
  Settings = this;

  _int_values["screen_width"]  = 1024;
  _int_values["screen_height"] = 768;

  _bool_values["tutorial"] = true;

  _bool_values["draw_collision"] = false;
  
  _bool_values["cheat_no_enemies"] = false;
  _bool_values["cheat_cheap_upgrades"] = false;
  _bool_values["cheat_planet_lander_disable_distance_check"] = false;
  
  return true;
}


void SubsystemSettings::Stop()
{
  if(Settings == this)
    Settings = nullptr;
}


bool SubsystemSettings::GetBool(const std::string & name) const
{
  auto it = _bool_values.find(name);
  if(it != _bool_values.end())
    return (*it).second;

  assert(false);
  return false;
}


int SubsystemSettings::GetInt(const std::string & name) const
{
  auto it = _int_values.find(name);
  if(it != _int_values.end())
    return (*it).second;

  assert(false);
  return 0;
}


double SubsystemSettings::GetDouble(const std::string & name) const
{
  auto it = _int_values.find(name);
  if(it != _int_values.end())
    return (*it).second;

  assert(false);
  return 0;
}
