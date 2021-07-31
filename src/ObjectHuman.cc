/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
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
