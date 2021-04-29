#include "glm.hh"

std::ostream & operator<<(std::ostream & out, const glm::vec3 & v)
{
  const char delimiter = ' ';

  out << v.x << delimiter;
  out << v.y << delimiter;
  out << v.z << delimiter;

  return out;
}
