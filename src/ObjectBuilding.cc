#include "ObjectBuilding.hh"
#include "SubsystemAssetLoader.hh"


ObjectBuilding::ObjectBuilding(Scene * scene, unsigned int random_seed, unsigned int type)
  : Object(scene, random_seed)
{
  SetIsSleeping(true);
  
  SetCollisionChannels(0);
  AddToCollisionChannel(Object::CollisionChannel::TERRAIN);
  SetCollidesWithChannels(0);
  AddCollidesWithChannel(Object::CollisionChannel::ENEMY);
  AddCollidesWithChannel(Object::CollisionChannel::PLAYER);

  auto config = AssetLoader->LoadJson("Data/Buildings");
  assert(config->is_object());
  assert((*config)["definitions"].is_array());
  auto defs = (*config)["definitions"].array_items();
  assert(type < defs.size());

  auto d = defs[type];
  
  assert(d["mesh"].is_string());
  auto mesh = AssetLoader->LoadMesh(d["mesh"].string_value());
  assert(mesh);
  SetMesh(mesh);

  CollisionShape::Type ct = CollisionShape::Type::OBB;
  if(d["collision_shape"].is_string())
    {
      if(d["collision_shape"] == "obb")
        ct = CollisionShape::Type::OBB;
      else if(d["collision_shape"] == "sphere")
        ct = CollisionShape::Type::SPHERE;
      else
        assert(false); // unknown collision shape
    }

  CreateCollisionShape(ct);

  assert(d["health"].is_number());
  SetMaxHealth(d["health"].number_value());
  SetHealth(GetMaxHealth());
}
