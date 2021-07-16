#ifndef SUBSYSTEM_HIGHSCORES_HH_
#define SUBSYSTEM_HIGHSCORES_HH_
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
#include <vector>

class HighscoreEntry;


class SubsystemHighscores : public Subsystem
{
public:
  SubsystemHighscores();

  bool Start() override;
  void Stop() override;

  void Add(HighscoreEntry * entry);
  const std::vector<HighscoreEntry *> & Get() const;

  void Read();
  void Write();

private:
  std::vector<HighscoreEntry *> _entries;
};

extern SubsystemHighscores * Highscores;

#endif
