/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectLevelEntrance.hh"
#include "CollisionShapeSphere.hh"
#include "Mesh.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"


ObjectLevelEntrance::ObjectLevelEntrance(Scene * scene, SolarSystemObject * planet)
  : ObjectMovable(scene, 0),
    _planet(planet),
    _used(false)
{
  SetMesh(AssetLoader->LoadMesh("LevelEntrance"));
  GetMesh()->SetShaderProgram(AssetLoader->LoadShaderProgram("LevelEntrance"), true);
  SetCollisionShape(new CollisionShapeSphere(this, 0.5));
  SetCollisionChannels(0);
  SetCollidesWithChannels(0);
  AddCollidesWithChannel(CollisionChannel::PLAYER);
  SetIsAffectedByGravity(false);
  SetTickingRequiresPlayerAlive(true);
  //  SetColor({1.0, 0.5, 0.5, 0.2});
}


void ObjectLevelEntrance::OnCollision(Object & other, [[maybe_unused]] const glm::vec3 & hit_direction)
{
  if(_used)
    return;

  auto player = dynamic_cast<ObjectSpaceship *>(&other);
  if(!player)
    return;

  if(!Settings->GetBool("cheat_disable_planet_lander_check"))
    if(player->GetUpgrade(SpaceshipUpgrade::Type::PLANET_LANDER)->GetInstallCount() == 0)
      return;
  
  assert(player->GetOwnerGameStats());
  
  _used = true;

  player->DescendToPlanet(_planet);
}
