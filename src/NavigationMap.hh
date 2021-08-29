#ifndef NAVIGATION_MAP_HH_
#define NAVIGATION_MAP_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"


class NavigationMap
{
public:
  NavigationMap(const glm::ivec2 & data_dimensions, uint8_t * data, const glm::vec2 scale);

  const glm::ivec2 & GetDataDimensions() const;
  const uint8_t *    GetData() const;
  uint8_t            Get(const glm::ivec2 & position) const;
  void               Set(const glm::ivec2 & position, uint8_t value) const;
    
  glm::vec2  NavigationToWorld(const glm::ivec2 & position);
  
  glm::ivec2 WorldToNavigation(const glm::vec3 & position);
  glm::ivec2 WorldToNavigation(const glm::vec2 & position);

private:
  glm::ivec2 _data_dimensions;
  uint8_t *  _data;
  glm::vec2  _scale;
  glm::vec2  _tile_offset;
};

#endif
