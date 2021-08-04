/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Weapon.hh"
#include "Mesh.hh"
#include "ObjectProjectile.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "SubsystemAssetLoader.hh"


Weapon::Weapon(Object * owner, const glm::vec3 & location, const glm::vec3 & projectile_direction)
  : _owner(owner),
    _location(location),
    _autofire(false),
    _heat(0),
    _last_fire_timer(0),
    _projectile_mesh(nullptr),
    _projectile_direction(projectile_direction)
{
  SetAmmo(AmmoType::KINETIC);
}


bool Weapon::CanFire() const
{
  if(_last_fire_timer > 0.0)
    return false;
  
  if(_heat > 1.0)
    return false;

  auto spaceship = dynamic_cast<ObjectSpaceship *>(_owner);
  if(spaceship)
    {
      auto em = spaceship->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER);
      if(em->IsActive())
        return false;
    }
  
  return true;
}


void Weapon::Fire()
{
  auto spaceship = dynamic_cast<ObjectSpaceship *>(_owner);
  assert(spaceship);

  auto damage = _projectile_damage;
  auto bonus = spaceship->GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE);
  if(bonus->IsActive())
    damage *= bonus->GetValue();

  auto orientation = glm::toMat4(glm::inverse(spaceship->GetOrientation()));
  auto location = glm::vec4(_location, 1) * orientation;
  auto direction = glm::vec4(_projectile_direction, 1) * orientation;
  auto projectile = spaceship->GetScene()->AddProjectile(spaceship,
                                                         spaceship->GetPosition() + location.xyz(),
                                                         spaceship->GetVelocity() * 0.5f + direction.xyz() * static_cast<float>(_projectile_initial_velocity),
                                                         damage,
                                                         5.0);
  assert(_projectile_mesh);
  projectile->SetMesh(_projectile_mesh);
  projectile->SetIsAffectedByGravity(_projectile_use_gravity);
  projectile->SetGlow(_projectile_glow);
  
  _heat += _firing_heat;
  _last_fire_timer = _minimum_firing_interval;
}


bool Weapon::IsAutofireOn() const
{
  return _autofire;
}


void Weapon::SetAutofire(bool on)
{
  _autofire = on;
}

    
void Weapon::Tick(double deltatime, double coolingmult)
{
  _heat = std::max(0.0, _heat - 0.1 * coolingmult * deltatime);
  
  if(_last_fire_timer > 0.0)
    _last_fire_timer -= deltatime;
}


double Weapon::GetHeat() const
{
  return _heat;
}


void Weapon::SetOwner(Object * owner)
{
  _owner = owner;
}


void Weapon::SetAmmo(AmmoType type)
{
  _ammotype = type;
  switch(type)
    {
    case AmmoType::KINETIC:
      {
        _projectile_initial_velocity = 10;
        _projectile_damage = 34;
        _projectile_mesh = AssetLoader->LoadMesh("Projectile");
        _projectile_glow = 0;
        _minimum_firing_interval = 0.02;
        _firing_heat = 0.03;
        _projectile_use_gravity = true;
      }
      break;
    case AmmoType::PLASMA:
      {
        auto mesh = new Mesh(*AssetLoader->LoadMesh("Planet"));
        mesh->SetTexture(0, AssetLoader->LoadImage("8k_sun"), true);
        mesh->ApplyTransform(glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)));
        mesh->UpdateGPU();
        _projectile_initial_velocity = 20;
        _projectile_damage = 100;
        _projectile_mesh = mesh;
        _projectile_glow = 1;
        _minimum_firing_interval = 0.09;
        _firing_heat = 0.1;
        _projectile_use_gravity = false;
      }
      break;
    }
}


Weapon::AmmoType Weapon::GetAmmo() const
{
  return _ammotype;
}

