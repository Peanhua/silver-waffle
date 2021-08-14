/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif
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

  
  _int_values["texture_quality"] = 1; // Quality values go from low to high [0,1].
  
  _bool_values["draw_collision"] = false;

  _bool_values["demo"] = false;
  
  _bool_values["cheat_no_enemies"] = false;
  _bool_values["cheat_cheap_upgrades"] = true;
  _bool_values["cheat_disable_planet_lander_check"] = true;
  _bool_values["cheat_disable_planet_entering_impulse"] = false;
  
  _string_values["font"] = "Fonts/bitstream-vera-sans-mono-fonts/VeraMono.ttf";


#ifdef WITH_VALGRIND
  _int_values["texture_quality"] = 0;
#endif
  
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


void SubsystemSettings::SetBool(const std::string & name, bool value)
{
  _bool_values[name] = value;
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


const std::string & SubsystemSettings::GetString(const std::string & name) const
{
  auto it = _string_values.find(name);
  if(it != _string_values.end())
    return (*it).second;

  assert(false);
  static std::string defaultrv;
  return defaultrv;
}
