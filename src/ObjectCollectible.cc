#include "ObjectCollectible.hh"
#include "GameStats.hh"
#include "Mesh.hh"
#include "Scene.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"

ObjectCollectible::ObjectCollectible(Scene * scene)
  : ObjectMovable(scene)
{
  SetHealth(1);
  AddToCollisionChannel(CollisionChannel::COLLECTIBLE);
  AddCollidesWithChannel(CollisionChannel::PLAYER);
}


void ObjectCollectible::SetBonus(Type type, double bonus)
{
  _bonuses[type] = bonus;
}


bool ObjectCollectible::HasBonus(Type type) const
{
  return GetBonus(type) > 0.0;
}


double ObjectCollectible::GetBonus(Type type) const
{
  auto it = _bonuses.find(type);
  if(it == _bonuses.end())
    return 0.0;
  
  return (*it).second;
}


void ObjectCollectible::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  assert(&other == &other);
  assert(hit_direction == hit_direction);

  auto player = dynamic_cast<ObjectSpaceship *>(&other);
  assert(player);
  if(!player)
    return;
  
  player->UpgradeFromCollectible(this);

  auto gamestats = player->GetOwnerGameStats();
  assert(gamestats);
  if(!gamestats)
    return;
  
  if(HasBonus(Type::TYPE_SCORE_BONUS))
    {
      auto score = static_cast<unsigned int>(GetBonus(Type::TYPE_SCORE_BONUS));
      gamestats->AddScore(score);
    }
  if(HasBonus(Type::TYPE_SCORE_MULTIPLIER))
    {
      auto mult = static_cast<unsigned int>(GetBonus(Type::TYPE_SCORE_MULTIPLIER));
      gamestats->SetScoreMultiplier(mult, 30);
    }
  {
    UpgradeMaterial::Type upg_types[] =
      {
        UpgradeMaterial::Type::ATTACK,
        UpgradeMaterial::Type::DEFENSE,
        UpgradeMaterial::Type::PHYSICAL
      };
    Type my_types[] =
      {
        Type::TYPE_UPGRADEMATERIAL_ATTACK,
        Type::TYPE_UPGRADEMATERIAL_DEFENSE,
        Type::TYPE_UPGRADEMATERIAL_PHYSICAL
      };
    for(int i = 0; i < 3; i++)
      if(HasBonus(my_types[i]))
        {
          auto upgrade = gamestats->GetUpgradeMaterial(upg_types[i]);
          upgrade->Add(static_cast<unsigned int>(GetBonus(my_types[i])));
        }
  }
}


