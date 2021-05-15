#ifndef EXPLOSION_HH_
#define EXPLOSION_HH_

#include "glm.hh"
#include <random>

class Mesh;


class Explosion
{
public:
  Explosion(std::minstd_rand & random);

  void Activate(const glm::vec3 & position);
  void Tick(double deltatime);
  void Draw(const glm::mat4 & mvp) const;
  bool IsAlive() const;

private:
  int    _fragment_count;
  Mesh * _mesh;
  double _time;
};

#endif
