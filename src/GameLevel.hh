#ifndef GAMELEVEL_HH_
#define GAMELEVEL_HH_

#include "Object.hh"

class GameLevel : public Object
{
public:
  using Object::Object;
  
  virtual void Initialize(double difficulty);
  
private:
};


class GameLevel2 : public GameLevel
{
public:
  using GameLevel::GameLevel;

  void Initialize(double difficulty) override;
};

#endif
