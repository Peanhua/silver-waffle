#ifndef GLM_HH_
#define GLM_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <ostream>

std::ostream & operator<<(std::ostream & out, const glm::ivec2 & v);
std::ostream & operator<<(std::ostream & out, const glm::vec2 & v);
std::ostream & operator<<(std::ostream & out, const glm::vec3 & v);
std::ostream & operator<<(std::ostream & out, const glm::vec4 & v);
std::ostream & operator<<(std::ostream & out, const glm::mat4 & m);
std::ostream & operator<<(std::ostream & out, const glm::quat & q);

#endif
