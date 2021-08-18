/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "NavigationMap.hh"

NavigationMap::NavigationMap(const glm::ivec2 & data_dimensions, uint8_t * data, const glm::vec2 scale)
  : _data_dimensions(data_dimensions),
    _data(data),
    _scale(scale)
{
  _tile_offset = 0.5f * _scale / glm::vec2(_data_dimensions); // Tile origin is offset by 0.5*blocksize.
}

const glm::ivec2 & NavigationMap::GetDataDimensions() const
{
  return _data_dimensions;
}

const uint8_t * NavigationMap::GetData() const
{
  return _data;
}

uint8_t NavigationMap::GetData(const glm::ivec2 & position) const
{
  if(position.x >= 0 && position.x < _data_dimensions.x && position.y >= 0 && position.y < _data_dimensions.y)
    return _data[position.x + position.y * _data_dimensions.x];
  return 'X';
}

glm::vec2 NavigationMap::NavigationToWorld(const glm::ivec2 & position)
{
  auto rv = glm::vec2(position);
  rv /= glm::vec2(_data_dimensions);
  assert(rv.x >= 0);
  assert(rv.x <= 1);
  assert(rv.y >= 0);
  assert(rv.y <= 1);
  rv -= glm::vec2(0.5f, 0.5f); // Move to origin, world origin is at the center.
  rv *= _scale;
  rv += _tile_offset;
  return rv;
}


glm::ivec2 NavigationMap::WorldToNavigation(const glm::vec3 & position)
{
  return WorldToNavigation(position.xz());
}

glm::ivec2 NavigationMap::WorldToNavigation(const glm::vec2 & position)
{
  auto pos = position;
  pos -= _tile_offset;
  pos /= _scale;
  pos += glm::vec2(0.5f, 0.5f); // Move to origin, navigation map origin is at the bottom-left.

  pos.x = std::clamp(pos.x, 0.0f, 1.0f);
  pos.y = std::clamp(pos.y, 0.0f, 1.0f);
  pos *= glm::vec2(_data_dimensions);
  return glm::ivec2(pos);
}
