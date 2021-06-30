#ifndef HIGHSCORE_ENTRY_HH_
#define HIGHSCORE_ENTRY_HH_

#include <json11.hpp>
#include <string>

class Scene;


class HighscoreEntry
{
public:
  HighscoreEntry(unsigned int score, unsigned int level, Scene * scene, bool game_completed);
  HighscoreEntry(const json11::Json & json);

  unsigned int GetScore() const;

  json11::Json ToJson() const;
  
private:
  unsigned int _level;
  double       _level_time; // Time within the ending level.
  double       _total_time;
  unsigned int _score;
  bool         _game_completed;
};

#endif
