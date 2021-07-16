#ifndef OBJECT_INVADER_HH_
#define OBJECT_INVADER_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "ObjectSpaceship.hh"
#include "CollisionShape.hh"


class ObjectInvader : public ObjectSpaceship
{
public:
  ObjectInvader(Scene * scene, unsigned int random_seed);

  void OnDestroyed(Object * destroyer) override;
  void SetInvaderType(unsigned int type);
  void CreateCollisionShape(CollisionShape::Type type);
};

#endif
