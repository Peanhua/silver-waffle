#ifndef OBJECT_BUILDING_HH_
#define OBJECT_BUILDING_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Object.hh"


class ObjectBuilding : public Object
{
public:
  ObjectBuilding(Scene * scene, unsigned int random_seed, unsigned int type);

  void Tick(double deltatime) override;
  void OnDestroyed(Object * destroyer) override;

  void AddSpawn(unsigned int spawn_type, double spawn_interval, unsigned int max_count, const std::string & control_program);
  bool GetIsSpaceport() const;

private:
  struct SpawnEntry
  {
    SpawnEntry(unsigned int type_, double interval_, double next_spawn_time_, unsigned int max_count_, const std::string & control_program_)
      : type(type_),
        interval(interval_),
        next_spawn_time(next_spawn_time_),
        count(0),
        max_count(max_count_),
        control_program(control_program_)
    {
    }
    
    unsigned int type;
    double       interval;
    double       next_spawn_time;
    unsigned int count;
    unsigned int max_count;
    std::string  control_program;
  };

  std::vector<SpawnEntry> _spawns;
  bool   _is_spaceport;
};

#endif
