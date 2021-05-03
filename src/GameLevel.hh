#ifndef GAMELEVEL_HH_
#define GAMELEVEL_HH_

#include "Object.hh"
#include "ObjectMovable.hh"
#include "ObjectBullet.hh"

class GameLevel : public Object
{
public:
  GameLevel();
  
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


class GameLevel2 : public GameLevel
{
public:
  using GameLevel::GameLevel;

  void Initialize(double difficulty) override;
};

#endif
