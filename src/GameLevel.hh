#ifndef GAMELEVEL_HH_
#define GAMELEVEL_HH_

#include "Object.hh"
#include "ObjectMovable.hh"

class GameLevel : public Object
{
public:
  using Object::Object;
  
  virtual void Initialize(double difficulty);

  ObjectMovable * GetPlayer() const;
  
private:
  ObjectMovable *              _player;
  std::vector<ObjectMovable *> _invaders;
};


class GameLevel2 : public GameLevel
{
public:
  using GameLevel::GameLevel;

  void Initialize(double difficulty) override;
};

#endif
