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


SolarSystemObject::SolarSystemObject(Type type, const json11::Json & config)
  : _type(type),
    _ring(0, 0),
    _cloud_range(-1, 1)
{
  _name = config["name"].string_value();
  _relative_size = config["radius"].number_value() / 6371.0;
  _rotation_period = config["rotation"].number_value();
  _gravity = config["gravity"].number_value();
  _texture = AssetLoader->LoadImage(config["texture"].string_value());
  assert(_texture);
  if(config["cloud_range"].is_array())
    {
      auto cloudrange = config["cloud_range"].array_items();
      _cloud_range.x = static_cast<float>(cloudrange[0].number_value());
      _cloud_range.y = static_cast<float>(cloudrange[1].number_value());
      assert(_cloud_range.x <= _cloud_range.y);
      assert(_cloud_range.x >= 0 && _cloud_range.x <= 1);
      assert(_cloud_range.y >= 0 && _cloud_range.y <= 1);
    }

  {
    auto colors = config["atmosphere"].array_items();
    assert(colors.size() > 1);
    for(auto c : colors)
      {
        glm::vec3 color(c["r"].number_value(), c["g"].number_value(), c["b"].number_value());
        _atmosphere_colors.push_back(color);
      }
    assert(_atmosphere_colors.size() > 1);
  }

  if(config["ring"].is_array())
    {
      auto a = config["ring"].array_items();
      assert(a.size() == 2);
      _ring.x = static_cast<float>(a[0].number_value());
      _ring.y = static_cast<float>(a[1].number_value());
    }
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

  auto amount = 1.0 + 1.0 / std::sqrt(_rotation_period) * 5.0;
  obj->SetAngularVelocity(glm::angleAxis(glm::radians(1.0f), glm::vec3(0, 0, 1)), amount);
  
  return obj;
}


const std::string & SolarSystemObject::GetName() const
{
  return _name;
}


double SolarSystemObject::GetGravity() const
{
    return _gravity;
}


Image * SolarSystemObject::GetTexture() const
{
  return _texture;
}

const std::vector<glm::vec3> & SolarSystemObject::GetAtmosphereColors() const
{
  return _atmosphere_colors;
}


const glm::vec2 & SolarSystemObject::GetCloudRange() const
{
  return _cloud_range;
}
