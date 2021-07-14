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
  : ObjectSpaceship(scene, random_seed)
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
    EnableHitImpulse(d["hit_impulse"].bool_value());

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
  if(static_cast<double>(GetRand()) < d["shield_chance"].number_value())
    {
      auto u = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
      u->Activate(50.0 + GetMaxHealth() * static_cast<double>(GetRand()), 9999);
    }
  
  SetHealth(GetMaxHealth());
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
