#ifndef SPACE_PARTICLES_HH_
#define SPACE_PARTICLES_HH_
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

class Camera;
class Mesh;


class SpaceParticles
{
public:
  SpaceParticles(double radius_min, double radius_max, unsigned long random_seed, float startpos, float endpos);
  ~SpaceParticles();
  
  void Tick(double deltatime);
  void Draw(const Camera & camera) const;
  void SetMode(bool lines, float particle_length = 0.0f);
  
private:
  double _radius_min;
  double _radius_max;
  float  _startpos;
  float  _endpos;
  Mesh * _mesh;
  double _time;
  double _add_star_timer;
  float  _particle_length;
  unsigned int _star_count;
  unsigned int _next_index;
  std::mt19937_64                        _random;
  std::uniform_real_distribution<double> _rdist;

  double GetRandom(double min = 0.0);
  void ResetStar(unsigned int index);
};

#endif
