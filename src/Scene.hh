#ifndef SCENE_HH_
#define SCENE_HH_

#include "Object.hh"
#include "ObjectMovable.hh"
#include "ObjectBullet.hh"

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
  void              AddBullet(const glm::vec3 & position, const glm::vec3 & velocity, double lifetime);
  
private:
  ObjectSpaceship *              _player;
  std::vector<ObjectInvader *>   _invaders;
  std::vector<ObjectBullet *>    _bullets;
  unsigned int                   _bulletpos;
};


#endif
