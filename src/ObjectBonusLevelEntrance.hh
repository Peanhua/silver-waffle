#ifndef OBJECT_BONUS_LEVEL_ENTRANCE_HH_
#define OBJECT_BONUS_LEVEL_ENTRANCE_HH_

#include "ObjectMovable.hh"
#include <map>


class ObjectBonusLevelEntrance : public ObjectMovable
{
public:
  ObjectBonusLevelEntrance(Scene * scene, double enemy_difficulty, double warp_fuel_bonus);

  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
  
private:
  double _enemy_difficulty;
  double _warp_fuel_bonus;
  double _drag_timer;
  bool   _used;
};

#endif
