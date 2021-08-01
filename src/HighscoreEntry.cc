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


HighscoreEntry::HighscoreEntry(Scene * scene, unsigned int level, bool game_completed)
  : _level(level),
    _level_time(scene->GetTime()),
    _game_completed(game_completed)
{
  auto gamestats = scene->GetPlayer()->GetOwnerGameStats();
  _total_time = gamestats->GetTime();
  _score = gamestats->GetScore();
  _humans_collected = gamestats->GetHumansCollected();
  _humans_saved = gamestats->GetHumansSaved();
}


HighscoreEntry::HighscoreEntry(const json11::Json & json)
  : _humans_collected(0),
    _humans_saved(0)
{
  assert(json["score"].is_number());
  _score = static_cast<unsigned int>(json["score"].int_value());

  assert(json["level"].is_number());
  _level = static_cast<unsigned int>(json["level"].int_value());

  assert(json["level_time"].is_number());
  _level_time = json["level_time"].number_value();

  assert(json["total_time"].is_number());
  _total_time = json["total_time"].number_value();

  assert(json["game_completed"].is_bool());
  _game_completed = json["game_completed"].bool_value();

  if(json["humans_collected"].is_number())
    _humans_collected = static_cast<unsigned int>(json["humans_collected"].int_value());

  if(json["humans_saved"].is_number())
    _humans_saved = static_cast<unsigned int>(json["humans_saved"].int_value());
}


unsigned int HighscoreEntry::GetScore() const
{
  return _score;
}


double HighscoreEntry::GetGameTime() const
{
  return _total_time;
}


bool HighscoreEntry::GameCompleted() const
{
  return _game_completed;
}
    

json11::Json HighscoreEntry::ToJson() const
{
  return json11::Json::object
    {
      { "score",            static_cast<int>(_score) },
      { "level",            static_cast<int>(_level) },
      { "level_time",       _level_time },
      { "total_time",       _total_time },
      { "game_completed",   _game_completed },
      { "humans_collected", static_cast<int>(_humans_collected) },
      { "humans_saved",     static_cast<int>(_humans_saved) }
    };
}
