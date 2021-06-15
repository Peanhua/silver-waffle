#ifndef OBJECT_INVADER_HH_
#define OBJECT_INVADER_HH_

#include "ObjectSpaceship.hh"
#include <random>

class ObjectInvader : public ObjectSpaceship
{
public:
  ObjectInvader(Scene * scene, unsigned int random_seed);

  void Tick(double deltatime)          override;
  void OnDestroyed(Object * destroyer) override;
  
private:
  std::mt19937_64 _random_generator;
  double          _time_to_think;
  double          _next_thinking;

  void SpawnRandomCollectible();
};

#endif
