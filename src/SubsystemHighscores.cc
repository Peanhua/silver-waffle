/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SubsystemHighscores.hh"
#include "HighscoreEntry.hh"
#include "SubsystemAssetLoader.hh"
#include <algorithm>
#include <fstream>


SubsystemHighscores * Highscores = nullptr;


SubsystemHighscores::SubsystemHighscores()
  : Subsystem("Highscores"),
    _last(nullptr)
{
}


bool SubsystemHighscores::Start()
{
  if(Highscores)
    return false;

  Read();
  Highscores = this;

  return true;
}


void SubsystemHighscores::Stop()
{
  Write();
  if(Highscores == this)
    Highscores = nullptr;
}


void SubsystemHighscores::Add(HighscoreEntry * entry)
{
  _entries.push_back(entry);
  std::sort(_entries.begin(), _entries.end(),
            [](const HighscoreEntry * a, const HighscoreEntry * b)
            {
              return a->GetScore() > b->GetScore();
            });
}


const std::vector<HighscoreEntry *> & SubsystemHighscores::Get() const
{
  return _entries;
}


void SubsystemHighscores::Read()
{
  auto data = AssetLoader->LoadJson("Data/Highscores");
  if(!(*data)["entries"].is_array())
    return;

  for(auto entryjson : (*data)["entries"].array_items())
    Add(new HighscoreEntry(entryjson));
}


void SubsystemHighscores::Write()
{
  std::vector<json11::Json> entries;
  for(auto e : _entries)
    entries.push_back(e->ToJson());
  
  json11::Json data = json11::Json::object
    {
      { "entries", json11::Json(entries) }
    };
  
  std::ofstream file(AssetLoader->GetSaveRoot() + "/Data/Highscores.json");
  file << data.dump();
}



void SubsystemHighscores::SetLast(HighscoreEntry * last)
{
  _last = last;
}


HighscoreEntry * SubsystemHighscores::GetLast() const
{
  return _last;
}
