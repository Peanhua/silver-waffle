#include "ObjectInvader.hh"
#include "CollisionShapeOBB.hh"
#include "CollisionShapeSphere.hh"
#include "Controller.hh"
#include "GameStats.hh"
#include "Loot.hh"
#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"


ObjectInvader::ObjectInvader(Scene * scene, unsigned int random_seed)
  : ObjectSpaceship(scene),
    _random_generator(random_seed),
    _time_to_think(0.334),
    _next_thinking(0),
    _disable_hit_impulse(false)
{
  AddToCollisionChannel(CollisionChannel::ENEMY);
  AddCollidesWithChannel(CollisionChannel::PLAYER);
}


void ObjectInvader::OnDestroyed(Object * destroyer)
{
  if(destroyer)
    {
      auto controller = destroyer->GetController();
      if(controller)
        {
          auto player = controller->GetOwner();
          if(player)
            {
              auto gamestats = player->GetOwnerGameStats();
              if(gamestats)
                gamestats->AddScore(1);
            }
        }
    }

  ObjectMovable::OnDestroyed(destroyer);
}


void ObjectInvader::SetInvaderType(unsigned int type)
{
  auto rand = [this]()
  {
    return (static_cast<double>(_random_generator()) - static_cast<double>(_random_generator.min())) / static_cast<double>(_random_generator.max());
  };

  auto config = AssetLoader->LoadJson("Data/Invaders");
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
  
  RemoveWeapons();
  assert(d["weapons"].is_number());
  for(int i = 0; i < d["weapons"].number_value(); i++)
    AddWeapon();

  assert(d["health"].is_number());
  SetMaxHealth(d["health"].number_value());

  if(d["hit_impulse"].is_bool())
    _disable_hit_impulse = !d["hit_impulse"].bool_value();

  ClearLoot();
  assert(d["loot"].is_array());
  auto loots = d["loot"].array_items();
  for(auto loot : loots)
    {
      assert(loot.is_string());
      auto lootjson = AssetLoader->LoadJson("Data/Loot-" + loot.string_value());
      AddLoot(new Loot(lootjson));
    }
  assert(d["shield_chance"].is_number());
  if(rand() < d["shield_chance"].number_value())
    {
      auto u = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
      u->Activate(50.0 + GetMaxHealth() * rand(), 9999);
    }
  
  SetHealth(GetMaxHealth());
}


void ObjectInvader::Hit(Object * perpetrator, double damage, const glm::vec3 & impulse)
{
  if(_disable_hit_impulse)
    ObjectSpaceship::Hit(perpetrator, damage, glm::vec3(0, 0, 0));
  else
    ObjectSpaceship::Hit(perpetrator, damage, impulse);
}


void ObjectInvader::CreateCollisionShape(CollisionShape::Type type)
{
  switch(type)
    {
    case CollisionShape::Type::OBB:
      SetCollisionShape(new CollisionShapeOBB(this, GetMesh()->GetBoundingBoxHalfSize()));
      break;
    case CollisionShape::Type::SPHERE:
      SetCollisionShape(new CollisionShapeSphere(this, GetMesh()->GetBoundingSphereRadius()));
      break;
    }
}
