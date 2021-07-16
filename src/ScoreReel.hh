#ifndef SCORE_REEL_HH_
#define SCORE_REEL_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "glm.hh"
#include <vector>

class Mesh;


class ScoreReel
{
public:
  ScoreReel(unsigned int drum_count);

  void Tick(double deltatime);
  void Draw() const;
  void SetScore(unsigned int score);
  unsigned int GetScore() const;
  
private:
  unsigned int        _drum_count;
  double              _drum_width;
  unsigned int        _score;
  Mesh *              _background;
  std::vector<Mesh *> _drums;
  std::vector<double> _drums_angles;
  std::vector<double> _drums_target_angles;

  Mesh * CreateDrum(const std::vector<Mesh *> numbers) const;
};

#endif
