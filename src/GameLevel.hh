#ifndef GAMELEVEL_HH_
#define GAMELEVEL_HH_

#include "Object.hh"

class GameLevel : public Object
{
public:
  using Object::Object;
  
  void Initialize(double difficulty);
  
private:
};

#endif
