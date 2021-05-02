#ifndef OBJECT_INVADER_HH_
#define OBJECT_INVADER_HH_

#include "ObjectMovable.hh"

class ObjectInvader : public ObjectMovable
{
public:
  ObjectInvader(const glm::vec3 & position);

  void Tick(double deltatime) override;
  
private:
  double _time_to_think;
  double _next_thinking;
};

#endif
