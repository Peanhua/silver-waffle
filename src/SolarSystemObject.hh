#ifndef SOLAR_SYSTEM_OBJECT_HH_
#define SOLAR_SYSTEM_OBJECT_HH_

#include "glm.hh"

class Image;
class Object;
class Scene;


class SolarSystemObject
{
public:
  enum Type
    {
      TYPE_STAR,
      TYPE_PLANET
    };

  SolarSystemObject(Type type, const std::string & texture, double radius, const glm::vec2 & ring = glm::vec2(0, 0));

  double   GetRelativeSize() const;
  
  Object * CreateSceneObject(Scene * scene, double scale) const;

  
private:
  Type      _type;
  double    _relative_size;
  glm::vec2 _ring;
  Image *   _texture;
};

#endif
