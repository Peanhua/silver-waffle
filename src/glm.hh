#ifndef GLM_HH_
#define GLM_HH_

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <ostream>

std::ostream & operator<<(std::ostream & out, const glm::vec3 & v);
std::ostream & operator<<(std::ostream & out, const glm::mat4 & m);

#endif
