#ifndef GLM_HH_
#define GLM_HH_

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
std::ostream & operator<<(std::ostream & out, const glm::vec3 & v);
std::ostream & operator<<(std::ostream & out, const glm::mat4 & m);
std::ostream & operator<<(std::ostream & out, const glm::quat & q);

#endif
