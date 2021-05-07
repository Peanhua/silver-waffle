#ifndef OBJECT_INVADER_HH_
#define OBJECT_INVADER_HH_

#include "ObjectSpaceship.hh"

class ObjectInvader : public ObjectSpaceship
{
public:
  ObjectInvader(Scene * scene);

  void Tick(double deltatime) override;
  
private:
  double _time_to_think;
  double _next_thinking;
};

#endif
