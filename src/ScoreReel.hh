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

  bool Tick(double deltatime);
  void Draw() const;
  void SetScore(unsigned int score);
  unsigned int GetScore() const;
  
private:
  unsigned int        _drum_count;
  double              _drum_width;
  unsigned int        _score;
  Mesh *              _background;
  Mesh *              _drum;
  std::vector<double> _drums_angles;
  std::vector<double> _drums_target_angles;
  glm::mat4           _projection;
  glm::mat4           _view;
  glm::mat4           _viewprojection;
  glm::mat4           _model;
};

#endif
