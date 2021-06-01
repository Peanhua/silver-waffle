#include "ObjectCollectible.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"

ObjectCollectible::ObjectCollectible(Scene * scene)
  : ObjectMovable(scene)
{
}


void ObjectCollectible::SetBonus(Type type, double bonus)
{
  _bonuses[type] = bonus;
}

bool ObjectCollectible::HasBonus(Type type) const
{
  auto it = _bonuses.find(type);
  if(it == _bonuses.end())
    return false;
  
  return (*it).second > 0.0;
}

double ObjectCollectible::GetBonus(Type type) const
{
  auto it = _bonuses.find(type);
  if(it == _bonuses.end())
    return 0.0;
  
  return (*it).second;
}
