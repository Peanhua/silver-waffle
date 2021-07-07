#ifndef SOLAR_SYSTEM_OBJECT_HH_
#define SOLAR_SYSTEM_OBJECT_HH_

#include "glm.hh"

class Image;
class Object;
class Scene;


class SolarSystemObject
{
public:
  enum class Type
    {
      STAR,
      PLANET
    };

  SolarSystemObject(Type type, const std::string & name, const std::string & texture, double radius, const glm::vec2 & ring = glm::vec2(0, 0));

  double   GetRelativeSize() const;
  Type     GetType() const;
  const std::string & GetName() const;
  
  Object * CreateSceneObject(Scene * scene, double scale);

  
private:
  Type        _type;
  std::string _name;
  double      _relative_size;
  glm::vec2   _ring;
  Image *     _texture;
};

#endif
