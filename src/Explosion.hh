#ifndef EXPLOSION_HH_
#define EXPLOSION_HH_

#include "glm.hh"
#include <random>

class Mesh;


class Explosion
{
public:
  Explosion(std::minstd_rand & random);

  void Activate(const glm::vec3 & position, const glm::vec3 & velocity);
  void Tick(double deltatime);
  void Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const;
  bool IsAlive() const;
  void Translate(const glm::vec3 & translation);

private:
  int       _fragment_count;
  Mesh *    _mesh;
  double    _time;
  glm::vec3 _position;
  glm::vec3 _velocity;
};

#endif
