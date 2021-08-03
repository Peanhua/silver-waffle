#ifndef OBJECT_LEVEL_ENTRANCE_HH_
#define OBJECT_LEVEL_ENTRANCE_HH_
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

class SolarSystemObject;


class ObjectLevelEntrance : public ObjectMovable
{
public:
  ObjectLevelEntrance(Scene * scene, SolarSystemObject * planet);

  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
  
private:
  SolarSystemObject * _planet;
  bool                _used;
};

#endif
