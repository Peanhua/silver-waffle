#include "ObjectInvader.hh"
#include "CollisionShapeOBB.hh"
#include "CollisionShapeSphere.hh"
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
  auto player = dynamic_cast<ObjectSpaceship *>(destroyer);
  if(player)
    {
      auto gamestats = player->GetOwnerGameStats();
      if(gamestats)
        gamestats->AddScore(1);
      
      SpawnRandomCollectibles();
    }

  ObjectMovable::OnDestroyed(destroyer);
}


void ObjectInvader::SpawnRandomCollectibles()
{
  auto rand = [this] { return static_cast<double>(_random_generator() - _random_generator.min()) / static_cast<double>(_random_generator.max()); };

  for(auto loot : _lootset)
    {
      auto item = loot->CreateLootItem(rand(), rand());
      if(item)
        {
          auto c = AssetLoader->LoadObjectCollectible(static_cast<int>(item->_type));
          assert(c);
          auto coll = new ObjectCollectible(*c);
          coll->SetBonus(item->_type, item->_bonus);
          GetScene()->AddCollectible(coll,
                                     GetPosition() + 0.5f * glm::vec3(rand() - 0.5, rand() - 0.5, 0.0),
                                     glm::vec3(0, -5, 0));

          switch(item->_type)
            {
            case ObjectCollectible::Type::SCORE_BONUS:
            case ObjectCollectible::Type::UPGRADEMATERIAL_ATTACK:
            case ObjectCollectible::Type::UPGRADEMATERIAL_DEFENSE:
            case ObjectCollectible::Type::UPGRADEMATERIAL_PHYSICAL:
            case ObjectCollectible::Type::WARP_FUEL:
              {
                auto rotangle = glm::normalize(glm::vec3(rand() * 2.0 - 1.0,
                                                         rand() * 2.0 - 1.0,
                                                         rand() * 2.0 - 1.0));
                coll->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + rand() * 10.0);
              }
              break;
            case ObjectCollectible::Type::DAMAGE_MULTIPLIER:
            case ObjectCollectible::Type::SCORE_MULTIPLIER:
            case ObjectCollectible::Type::SHIELD:
            case ObjectCollectible::Type::NONE:
              break;
            }
        }
    }
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
  switch(ct)
    {
    case CollisionShape::Type::OBB:
      SetCollisionShape(new CollisionShapeOBB(this, GetMesh()->GetBoundingBoxHalfSize()));
      break;
    case CollisionShape::Type::SPHERE:
      SetCollisionShape(new CollisionShapeSphere(this, GetMesh()->GetBoundingSphereRadius()));
      break;
    }
  
  RemoveWeapons();
  assert(d["weapons"].is_number());
  for(int i = 0; i < d["weapons"].number_value(); i++)
    AddWeapon();

  assert(d["health"].is_number());
  SetMaxHealth(d["health"].number_value());

  if(d["hit_impulse"].is_bool())
    _disable_hit_impulse = !d["hit_impulse"].bool_value();

  _lootset.clear();
  assert(d["loot"].is_array());
  auto loots = d["loot"].array_items();
  for(auto loot : loots)
    {
      assert(loot.is_string());
      auto lootjson = AssetLoader->LoadJson("Data/Loot-" + loot.string_value());
      _lootset.push_back(new Loot(lootjson));
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
