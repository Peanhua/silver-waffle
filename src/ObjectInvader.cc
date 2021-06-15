#include "ObjectInvader.hh"
#include "GameStats.hh"
#include "ObjectCollectible.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"


ObjectInvader::ObjectInvader(Scene * scene, unsigned int random_seed)
  : ObjectSpaceship(scene),
    _random_generator(random_seed),
    _time_to_think(0.334),
    _next_thinking(0)
{
}


void ObjectInvader::Tick(double deltatime)
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

  ObjectSpaceship::Tick(deltatime);
}


void ObjectInvader::OnDestroyed(Object * destroyer)
{
  auto player = dynamic_cast<ObjectSpaceship *>(destroyer);
  if(player)
    {
      auto gamestats = player->GetOwnerGameStats();
      gamestats->AddScore(1);
      SpawnRandomCollectible();
    }

  ObjectMovable::OnDestroyed(destroyer);
}


void ObjectInvader::SpawnRandomCollectible()
{
  ObjectCollectible::Type bonustype = ObjectCollectible::Type::TYPE_NONE;
  double bonus;
  bool dorandomrotation = false;

  auto rand = [this] { return static_cast<double>(_random_generator() - _random_generator.min()) / static_cast<double>(_random_generator.max()); };
  
  double r = rand();
  if(r < 0.10)
    {
      bonustype = ObjectCollectible::Type::TYPE_SCORE_BONUS;
      bonus = 5.0 + rand() * 5.0;
      dorandomrotation = true;
    }
  else if(r < 0.15)
    {
      bonustype = ObjectCollectible::Type::TYPE_DAMAGE_MULTIPLIER;
      bonus = 2.0;
    }
  else if(r < 0.20)
    {
      bonustype = ObjectCollectible::Type::TYPE_SCORE_MULTIPLIER;
      bonus = 2.0;
    }
  else if(r < 0.30)
    {
      bonustype = ObjectCollectible::Type::TYPE_SHIELD;
      bonus = 100.0;
    }
  else if(r < 0.50)
    {
      bonustype = ObjectCollectible::Type::TYPE_UPGRADEMATERIAL_ATTACK;
      bonus = 1.0;
      dorandomrotation = true;
    }
  else if(r < 0.70)
    {
      bonustype = ObjectCollectible::Type::TYPE_UPGRADEMATERIAL_DEFENSE;
      bonus = 1.0;
      dorandomrotation = true;
    }
  else if(r < 0.90)
    {
      bonustype = ObjectCollectible::Type::TYPE_UPGRADEMATERIAL_PHYSICAL;
      bonus = 1.0;
      dorandomrotation = true;
    }

  if(bonustype != ObjectCollectible::Type::TYPE_NONE)
    {
      auto c = AssetLoader->LoadObjectCollectible(bonustype);
      assert(c);
      auto coll = new ObjectCollectible(*c);
      coll->SetBonus(bonustype, bonus);
      GetScene()->AddCollectible(coll, GetPosition(), glm::vec3(0, -5, 0));
      if(dorandomrotation)
        {
          auto rotangle = glm::normalize(glm::vec3(rand() * 2.0 - 1.0,
                                                   rand() * 2.0 - 1.0,
                                                   rand() * 2.0 - 1.0));
          coll->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + rand() * 10.0);
        }
    }
}
