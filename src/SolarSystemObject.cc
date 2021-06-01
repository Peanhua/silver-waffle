#include "SolarSystemObject.hh"
#include "ObjectPlanet.hh"
#include "SubsystemAssetLoader.hh"

#include <iostream>
SolarSystemObject::SolarSystemObject(Type type, const std::string & texture, double radius, const glm::vec2 & ring)
  : _type(type),
    _relative_size(radius / 6371.0),
    _ring(ring)
{
  _texture = AssetLoader->LoadImage(texture);
  assert(_texture);
}


double SolarSystemObject::GetRelativeSize() const
{
  return _relative_size;
}

  
Object * SolarSystemObject::CreateSceneObject(Scene * scene, double scale) const
{
  auto obj = new ObjectPlanet(scene, _texture, scale * _relative_size);
  if(_ring.y > 0.0f && _ring.x < _ring.y)
    obj->AddPlanetRing(_ring.x, _ring.y);

  return obj;
}
