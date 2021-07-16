/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "HighscoreEntry.hh"
#include "GameStats.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include <cassert>


HighscoreEntry::HighscoreEntry(unsigned int score, unsigned int level, Scene * scene, bool game_completed)
  : _level(level),
    _level_time(scene->GetTime()),
    _total_time(scene->GetPlayer()->GetOwnerGameStats()->GetTime()),
    _score(score),
    _game_completed(game_completed)
{
}


HighscoreEntry::HighscoreEntry(const json11::Json & json)
{
  assert(json["score"].is_number());
  _score = static_cast<unsigned int>(json["score"].number_value());

  assert(json["level"].is_number());
  _level = static_cast<unsigned int>(json["level"].number_value());

  assert(json["level_time"].is_number());
  _level_time = json["level_time"].number_value();

  assert(json["total_time"].is_number());
  _total_time = json["total_time"].number_value();

  assert(json["game_completed"].is_bool());
  _game_completed = json["game_completed"].bool_value();
}


unsigned int HighscoreEntry::GetScore() const
{
  return _score;
}


json11::Json HighscoreEntry::ToJson() const
{
  return json11::Json::object
    {
      { "score",          static_cast<int>(_score) },
      { "level",          static_cast<int>(_level) },
      { "level_time",     _level_time },
      { "total_time",     _total_time },
      { "game_completed", _game_completed }
    };
}
