#ifndef SUBSYSTEM_HIGHSCORES_HH_
#define SUBSYSTEM_HIGHSCORES_HH_

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
