#ifndef SUBSYSTEM_SETTINGS_HH_
#define SUBSYSTEM_SETTINGS_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Subsystem.hh"
#include <map>


class SubsystemSettings : public Subsystem
{
public:
  SubsystemSettings();

  bool Start() override;
  void Stop() override;

  bool   GetBool(const std::string & name) const;
  void   SetBool(const std::string & name, bool value);
  int    GetInt(const std::string & name) const;
  double GetDouble(const std::string & name) const;

private:
  std::map<std::string, bool> _bool_values;
  std::map<std::string, int>  _int_values;
};

extern SubsystemSettings * Settings;

#endif
