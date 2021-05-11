#include "glm.hh"

std::ostream & operator<<(std::ostream & out, const glm::vec3 & v)
{
  const char delimiter = ' ';

  out << v.x << delimiter;
  out << v.y << delimiter;
  out << v.z << delimiter;

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
