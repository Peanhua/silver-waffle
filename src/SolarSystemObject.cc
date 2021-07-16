/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SolarSystemObject.hh"
#include "ObjectPlanet.hh"
#include "SubsystemAssetLoader.hh"


SolarSystemObject::SolarSystemObject(Type type, const std::string & name, const std::string & texture, double radius, const glm::vec2 & ring)
  : _type(type),
    _name(name),
    _relative_size(radius / 6371.0),
    _ring(ring)
{
  _texture = AssetLoader->LoadImage(texture);
  assert(_texture);
}


double SolarSystemObject::GetRelativeSize() const
{
  return _relative_size;
}

  
Object * SolarSystemObject::CreateSceneObject(Scene * scene, double scale)
{
  auto obj = new ObjectPlanet(scene, this, _texture, scale * _relative_size);
  if(_ring.y > 0.0f && _ring.x < _ring.y)
    obj->AddPlanetRing(_ring.x, _ring.y);

  return obj;
}


const std::string & SolarSystemObject::GetName() const
{
  return _name;
}

