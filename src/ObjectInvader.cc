#include "ObjectInvader.hh"
#include "GameStats.hh"
#include "Loot.hh"
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
  AddCollidesWithChannel(CollisionChannel::PROJECTILE);
}


void ObjectInvader::Tick(double deltatime)
{
  if(GetActiveControlProgramCount() == 0)
    {
      _next_thinking -= deltatime;
      if(_next_thinking < 0.0)
        {
          _next_thinking += _time_to_think;
          auto vel = GetVelocity();
          glm::vec3 imp(0, 0, 0);
          bool doimp = false;
          
          if(vel.y > -2.0f)
            {
              imp.y = -0.5f;
              doimp = true;
            }
          if(vel.x < 0.0f)
            {
              imp.x = 0.1f;
              doimp = true;
            }
          else if(vel.x > 0.0f)
            {
              imp.x = -0.1f;
              doimp = true;
            }
          if(doimp)
            AddImpulse(imp);
          
          double r = static_cast<double>(_random_generator() - _random_generator.min()) / static_cast<double>(_random_generator.max());
          if(r < 0.01 * _time_to_think)
            FireWeapon(0);
        }
    }

  ObjectSpaceship::Tick(deltatime);
}


void ObjectInvader::OnDestroyed(Object * destroyer)
{
  auto player = dynamic_cast<ObjectSpaceship *>(destroyer);
  if(player)
    {
      auto gamestats = player->GetOwnerGameStats();
      assert(gamestats);
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
          auto c = AssetLoader->LoadObjectCollectible(item->_type);
          assert(c);
          auto coll = new ObjectCollectible(*c);
          coll->SetBonus(item->_type, item->_bonus);
          GetScene()->AddCollectible(coll,
                                     GetPosition() + 1.0f * glm::vec3(rand() - 0.5, rand() - 0.5, rand() - 0.5),
                                     glm::vec3(0, -5, 0));

          switch(item->_type)
            {
            case ObjectCollectible::Type::TYPE_SCORE_BONUS:
            case ObjectCollectible::Type::TYPE_UPGRADEMATERIAL_ATTACK:
            case ObjectCollectible::Type::TYPE_UPGRADEMATERIAL_DEFENSE:
            case ObjectCollectible::Type::TYPE_UPGRADEMATERIAL_PHYSICAL:
              auto rotangle = glm::normalize(glm::vec3(rand() * 2.0 - 1.0,
                                                       rand() * 2.0 - 1.0,
                                                       rand() * 2.0 - 1.0));
              coll->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + rand() * 10.0);
              break;
            }
        }
    }
}


void ObjectInvader::SetInvaderType(int type)
{
  switch(type)
    {
    case 0:
      {
        auto mesh = AssetLoader->LoadMesh("Invader1");
        assert(mesh);
        SetMesh(mesh);
        
        RemoveWeapons();
        AddWeapon();

        SetMaxHealth(100.0);
        _disable_hit_impulse = false;

        auto lootjson = AssetLoader->LoadJson("Data/Loot-Invader1");
        _lootset.clear();
        _lootset.push_back(new Loot(lootjson));
      }
      break;
    case 1:
      {
        auto mesh = AssetLoader->LoadMesh("Invader2");
        assert(mesh);
        SetMesh(mesh);
        
        RemoveWeapons();
        AddWeapon();
        AddWeapon();
        AddWeapon();

        SetMaxHealth(500.0);
        _disable_hit_impulse = true;

        auto lootjson = AssetLoader->LoadJson("Data/Loot-Boss1");
        _lootset.clear();
        _lootset.push_back(new Loot(lootjson));
        _lootset.push_back(new Loot(lootjson));
        _lootset.push_back(new Loot(lootjson));
      }
      break;
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
