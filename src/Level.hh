#ifndef LEVEL_HH_
#define LEVEL_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"
#include <random>
#include <string>
#include <vector>
#include <json11.hpp>

class Camera;
class Image;
class Mesh;
class Scene;


class Level
{
public:
  Level(Scene * scene);
  virtual ~Level();
  
  virtual void   Start();
  void           Stop();
  virtual void   Tick(double deltatime);

  bool                IsFinished() const;
  const std::string & GetName()    const;
  double              GetTime()    const;

  bool AreBossBuildingsAlive() const;
  
protected:
  class ProgramEntry
  {
  public:
    ProgramEntry();
    ProgramEntry(const json11::Json & config);

    void SetStartTime(double time);
    void SetStopTime(double time);
    void SetSpawnInterval(double interval);
    void SetInvaderControlProgram(const std::string & program_name);
    ProgramEntry * Tick(Scene * scene, double deltatime);
    void SetNext(ProgramEntry * next);
    double GetRemainingTime() const;
    
  private:
    double         _invader_spawn_timer;
    double         _invader_spawn_start_time;
    double         _invader_spawn_stop_time;
    double         _invader_spawn_interval;
    std::string    _invader_control_program;
    bool           _boss;
    unsigned int   _invader_type;
    unsigned int   _bosses_alive;
    unsigned int   _spawn_count;
    unsigned int   _max_spawn_count;
    ProgramEntry * _next;
  };
  
  Scene *                     _scene;
  bool                        _running;
  std::mt19937_64             _random_generator;
  std::uniform_real_distribution<float> _rdist;
  std::string                 _name;
  double                      _time;
  std::vector<ProgramEntry *> _program;
  bool                        _halt_without_program;
  json11::Json                _buildings_config;
  json11::Json                _enemy_config;
  Image *                     _destructible_terrain_config;
  unsigned int                _boss_buildings_alive;

  void LoadConfig(const std::string & filename);
};

#endif
