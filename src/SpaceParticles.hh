#ifndef SPACE_PARTICLES_HH_
#define SPACE_PARTICLES_HH_

#include "glm.hh"
#include <random>

class Camera;
class Mesh;


class SpaceParticles
{
public:
  SpaceParticles(double radius_min, double radius_max, unsigned long random_seed);

  void Tick(double deltatime);
  void Draw(const Camera & camera) const;

private:
  double _radius_min;
  double _radius_max;
  Mesh * _mesh;
  double _time;
  double _add_star_timer;
  unsigned int _star_count;
  unsigned int _next_index;
  std::mt19937_64                        _random;
  std::uniform_real_distribution<double> _rdist;

  double GetRandom(double min = 0.0);
  void ResetStar(unsigned int index);
};

#endif
