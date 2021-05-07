#ifndef SCENE_HH_
#define SCENE_HH_

#include "Object.hh"
#include "ObjectMovable.hh"
#include "ObjectBullet.hh"

class Scene : public Object
{
public:
  Scene();
  
  void            Draw(const glm::mat4 & mvp) const override;
  void            Tick(double deltatime) override;

  virtual void    Initialize(double difficulty);

  ObjectMovable * GetPlayer() const;
  void            AddPlayerBullet(const glm::vec3 & velocity, double lifetime);
  
private:
  ObjectMovable *              _player;
  std::vector<ObjectMovable *> _invaders;
  std::vector<ObjectBullet *>  _bullets;
  unsigned int                 _bulletpos;
};


#endif
