#include "ObjectHuman.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


ObjectHuman::ObjectHuman(Scene * scene, unsigned int random_seed)
  : ObjectMovable(scene, random_seed)
{
  SetMesh(AssetLoader->LoadMesh("Human"));
  SetCollisionShape(new CollisionShapeOBB(this, GetMesh()->GetBoundingBoxHalfSize()));
  AddToCollisionChannel(CollisionChannel::COLLECTIBLE);
  //  AddCollidesWithChannel(CollisionChannel::PLAYER);
}


void ObjectHuman::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  bool handled = false;
  auto player = dynamic_cast<ObjectSpaceship *>(&other);
  if(player && player->GetOwnerGameStats())
    {
      handled = true;

      auto humancount = player->GetInventory().size();
      if(humancount < 3)
        {
          player->SystemlogAppend("You pick up a survivor,\ntake them to the base!\n");
          GetScene()->RemoveObject(this);
          player->AddToInventory(this);
        }
    }

  if(!handled)
    ObjectMovable::OnCollision(other, hit_direction);
}
