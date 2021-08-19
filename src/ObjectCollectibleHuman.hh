#ifndef OBJECT_COLLECTIBLE_HUMAN_HH_
#define OBJECT_COLLECTIBLE_HUMAN_HH_

#include "ObjectCollectible.hh"


class ObjectCollectibleHuman : public ObjectCollectible
{
public:
  ObjectCollectibleHuman();

  void Tick(double deltatime) override;
  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
  
private:
  double _next_think_time;
  float  _moving_direction;
};

#endif
