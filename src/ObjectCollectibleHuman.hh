#ifndef OBJECT_COLLECTIBLE_HUMAN_HH_
#define OBJECT_COLLECTIBLE_HUMAN_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

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
