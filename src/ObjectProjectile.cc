/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectProjectile.hh"
#include "CollisionShapeSphere.hh"
#include "Mesh.hh"
#include "ObjectSpaceship.hh"
#include "QuadTree.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"
#include <array>


ObjectProjectile::ObjectProjectile(Scene * scene)
  : ObjectMovable(scene, 0)
{
  SetOwner(nullptr);
  SetMesh(AssetLoader->LoadMesh("Projectile"));
  SetCollisionShape(new CollisionShapeSphere(this, GetMesh()->GetBoundingSphereRadius()));
  AddToCollisionChannel(CollisionChannel::PROJECTILE);
  SetTickingRequiresPlayerVisibility(true);
  Destroy(nullptr);
}


void ObjectProjectile::Tick(double deltatime)
{
  ObjectMovable::Tick(deltatime);

  _lifetime_left -= deltatime;
  if(_lifetime_left < 0.0)
    Destroy(nullptr);
}


void ObjectProjectile::Activate(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime)
{
  assert(!IsAlive());
  Revive(1);
  SetPosition(position);
  SetVelocity(velocity);
  SetOwner(owner);
  _damage = damage;
  _lifetime_left = lifetime;
}


Object * ObjectProjectile::GetOwner() const
{
  return _owner;
}


void ObjectProjectile::SetOwner(Object * owner)
{
  _owner = owner;
  
  if(_owner)
    SetCollidesWithChannels(_owner->GetCollidesWithChannels());
}


double ObjectProjectile::GetDamage() const
{
  return _damage;
}


void ObjectProjectile::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  other.Hit(_owner, this, GetDamage(), GetPosition(), -hit_direction);
  Destroy(nullptr);
}


void ObjectProjectile::OnDestroyed(Object * destroyer)
{
  auto spaceship = dynamic_cast<ObjectSpaceship *>(_owner);
  if(spaceship)
    spaceship->OnFiredProjectileDestroyed(this);

  ObjectMovable::OnDestroyed(destroyer);
}
