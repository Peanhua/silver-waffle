#ifndef HIGHSCORE_ENTRY_HH_
#define HIGHSCORE_ENTRY_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <json11.hpp>
#include <string>

class Scene;


class HighscoreEntry
{
public:
  HighscoreEntry(unsigned int score, unsigned int level, Scene * scene, bool game_completed);
  HighscoreEntry(const json11::Json & json);

  unsigned int GetScore() const;
  bool         GameCompleted() const;
  double       GetGameTime() const;

  json11::Json ToJson() const;
  
private:
  unsigned int _level;
  double       _level_time; // Time within the ending level.
  double       _total_time;
  unsigned int _score;
  bool         _game_completed;
};

#endif
