#ifndef OBJECT_INVADER_HH_
#define OBJECT_INVADER_HH_

#include "ObjectSpaceship.hh"
#include <random>

class Loot;


class ObjectInvader : public ObjectSpaceship
{
public:
  ObjectInvader(Scene * scene, unsigned int random_seed);

  void Tick(double deltatime)          override;
  void OnDestroyed(Object * destroyer) override;
  void Hit(Object * perpetrator, double damage, const glm::vec3 & impulse) override;
  void SetInvaderType(int type);
  
private:
  std::mt19937_64 _random_generator;
  double          _time_to_think;
  double          _next_thinking;
  bool            _disable_hit_impulse;
  std::vector<Loot *> _lootset;

  void SpawnRandomCollectibles();
};

#endif
