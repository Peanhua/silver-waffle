#ifndef OBJECT_COLLECTIBLE_HH_
#define OBJECT_COLLECTIBLE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "ObjectMovable.hh"
#include <map>

class ObjectSpaceship;


class ObjectCollectible : public ObjectMovable
{
public:
  enum class Type
    {
      SCORE_BONUS,
      DAMAGE_MULTIPLIER,
      SCORE_MULTIPLIER,
      SHIELD,
      UPGRADEMATERIAL_ATTACK,
      UPGRADEMATERIAL_DEFENSE,
      UPGRADEMATERIAL_PHYSICAL,
      WARP_FUEL,
      HUMAN,
      SPACESHIP_UPGRADE_BLUEPRINT,
      SIZEOF_,
      NONE = SIZEOF_
    };
  
  ObjectCollectible(Scene * scene);

  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
  
  void   SetBonus(Type type, double amount);
  bool   HasBonus(Type type) const;
  double GetBonus(Type type) const;
  
private:
  std::map<Type, double> _bonuses;

  void CollectBy(ObjectSpaceship * spaceship);
};

#endif
