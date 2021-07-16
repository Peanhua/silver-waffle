#ifndef SOLAR_SYSTEM_OBJECT_HH_
#define SOLAR_SYSTEM_OBJECT_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "glm.hh"

class Image;
class Object;
class Scene;


class SolarSystemObject
{
public:
  enum class Type
    {
      STAR,
      PLANET
    };

  SolarSystemObject(Type type, const std::string & name, const std::string & texture, double radius, const glm::vec2 & ring = glm::vec2(0, 0));

  double   GetRelativeSize() const;
  Type     GetType() const;
  const std::string & GetName() const;
  
  Object * CreateSceneObject(Scene * scene, double scale);

  
private:
  Type        _type;
  std::string _name;
  double      _relative_size;
  glm::vec2   _ring;
  Image *     _texture;
};

#endif
