#ifndef SCENE_HH_
#define SCENE_HH_

#include "glm.hh"
#include <random>

class Explosion;
class Mesh;
class Object;
class ObjectInvader;
class ObjectProjectile;
class ObjectSpaceship;


class Scene
{
public:
  Scene();
  
  void              Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const;
  void              Tick(double deltatime);

  void              Initialize(double difficulty);

  ObjectSpaceship * GetPlayer() const;
  void              AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);
  void              AddExplosion(const glm::vec3 & position);
  
private:
  std::mt19937_64                 _random_generator;
  ObjectSpaceship *               _player;
  std::vector<ObjectInvader *>    _invaders;
  std::vector<ObjectProjectile *> _projectiles;
  unsigned int                    _projectilepos;
  std::vector<Explosion *>        _explosions;
  unsigned int                    _explosionpos;
  std::vector<Mesh *>             _numbers;
};


#endif
