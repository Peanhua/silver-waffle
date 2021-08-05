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


std::ostream & operator<<(std::ostream & out, const glm::ivec2 & v)
{
  const char delimiter = ' ';

  out << v.x << delimiter;
  out << v.y << delimiter;

  return out;
}

std::ostream & operator<<(std::ostream & out, const glm::vec2 & v)
{
  const char delimiter = ' ';

  out << v.x << delimiter;
  out << v.y << delimiter;

  return out;
}


std::ostream & operator<<(std::ostream & out, const glm::vec3 & v)
{
  const char delimiter = ' ';

  out << v.x << delimiter;
  out << v.y << delimiter;
  out << v.z << delimiter;

  return out;
}


std::ostream & operator<<(std::ostream & out, const glm::vec4 & v)
{
  const char delimiter = ' ';

  out << v.x << delimiter;
  out << v.y << delimiter;
  out << v.z << delimiter;
  out << v.w << delimiter;

  return out;
}


std::ostream & operator<<(std::ostream & out, const glm::mat4 & m)
{
  char delimiter = ' ';

  for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++)
      out << m[y][x] << delimiter;
  
  return out;
}


std::ostream & operator<<(std::ostream & out, const glm::quat & q)
{
  out << "(r=" << glm::roll(q) << ", p=" << glm::pitch(q) << ", y=" << glm::yaw(q) << ")";
  return out;
}
