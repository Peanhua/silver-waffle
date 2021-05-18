#ifndef SCORE_REEL_HH_
#define SCORE_REEL_HH_

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
