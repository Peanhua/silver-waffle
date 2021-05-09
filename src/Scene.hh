#ifndef SCENE_HH_
#define SCENE_HH_

#include "Object.hh"
#include "ObjectMovable.hh"
#include "ObjectProjectile.hh"
#include <random>

class ObjectSpaceship;
class ObjectInvader;

class Scene
{
public:
  Scene();
  
  void              Draw(const glm::mat4 & mvp) const;
  void              Tick(double deltatime);

  void              Initialize(double difficulty);

  ObjectSpaceship * GetPlayer() const;
  void              AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);
  
private:
  std::mt19937_64                 _random_generator;
  ObjectSpaceship *               _player;
  std::vector<ObjectInvader *>    _invaders;
  std::vector<ObjectProjectile *> _projectiles;
  unsigned int                    _projectilepos;
};


#endif
