#ifndef OBJECT_PLANET_HH_
#define OBJECT_PLANET_HH_
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

class Image;
class SolarSystemObject;


class ObjectPlanet : public ObjectMovable
{
public:
  ObjectPlanet(Scene * scene, SolarSystemObject * solar_system_object, Image * planet_texture, double planet_radius);
  ~ObjectPlanet();

  double GetVisualBoundingSphereRadius() const override;

  void AddPlanetRing(double start, double end);
  SolarSystemObject * GetSolarSystemObject() const;
  
private:
  SolarSystemObject * _solar_system_object;
  double              _planet_radius;
  double              _ring_max;
};

#endif
