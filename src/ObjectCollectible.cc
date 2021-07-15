#include "ObjectCollectible.hh"
#include "GameStats.hh"
#include "Mesh.hh"
#include "Scene.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"

ObjectCollectible::ObjectCollectible(Scene * scene)
  : ObjectMovable(scene, 0)
{
  SetHealth(1);
  AddToCollisionChannel(CollisionChannel::COLLECTIBLE);
  AddCollidesWithChannel(CollisionChannel::PLAYER);
  SetBounciness(2.0);
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
  auto player = dynamic_cast<ObjectSpaceship *>(&other);
  if(player && player->GetOwnerGameStats()) // todo: Fix to work for also invaders? The check for GameStats is to distinguish from destructible terrain, fix by making destructible terrain class not descendant of ObjectSpaceship.
    CollectBy(player);
  else
    ObjectMovable::OnCollision(other, hit_direction);
}


void ObjectCollectible::CollectBy(ObjectSpaceship * spaceship)
{
  Destroy(spaceship);
  
  spaceship->UpgradeFromCollectible(this);

  auto gamestats = spaceship->GetOwnerGameStats();
  if(!gamestats)
    return;
  
  if(HasBonus(Type::SCORE_BONUS))
    {
      auto score = static_cast<unsigned int>(GetBonus(Type::SCORE_BONUS));
      gamestats->AddScore(score);
      spaceship->SystemlogAppend("Score +" + std::to_string(score) + "\n");
    }
  if(HasBonus(Type::SCORE_MULTIPLIER))
    {
      auto mult = static_cast<unsigned int>(GetBonus(Type::SCORE_MULTIPLIER));
      gamestats->SetScoreMultiplier(mult, 30);
      spaceship->SystemlogAppend("Score multiplier: " + std::to_string(mult) + "\n");
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
        Type::UPGRADEMATERIAL_ATTACK,
        Type::UPGRADEMATERIAL_DEFENSE,
        Type::UPGRADEMATERIAL_PHYSICAL
      };
    for(int i = 0; i < 3; i++)
      if(HasBonus(my_types[i]))
        {
          auto upgrade = gamestats->GetUpgradeMaterial(upg_types[i]);
          auto amount = static_cast<unsigned int>(GetBonus(my_types[i]));
          upgrade->Add(amount);
          spaceship->SystemlogAppend("Material " + upgrade->GetName() + " +" + std::to_string(amount) + "\n");
          GetScene()->TutorialMessage(1, "Press TAB to open the upgrade menu.\n");
        }
  }
}
