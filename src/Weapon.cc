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
#include "SubsystemSfx.hh"


Weapon::Weapon(Object * owner, const glm::vec3 & location, const glm::vec3 & projectile_direction)
  : _owner(owner),
    _weapon_group(0),
    _consumes_ammo(false),
    _ammo(0),
    _ammo_max(12),
    _location(location),
    _autofire(false),
    _heat(0),
    _last_fire_timer(0),
    _projectile_mesh(nullptr),
    _projectile_glow(0),
    _projectile_direction(projectile_direction)
{
  SetAmmoType(AmmoType::KINETIC);
}


bool Weapon::CanFire() const
{
  if(_last_fire_timer > 0.0)
    return false;
  
  if(_heat > 1.0)
    return false;

  if(_consumes_ammo && _ammo == 0)
    return false;

  if(_ammotype == AmmoType::BOMB)
    if(glm::length2(_owner->GetScene()->GetGravity()) < 2)
      return false;

  return true;
}


ObjectProjectile * Weapon::Fire()
{
  switch(_ammotype)
    {
    case AmmoType::BOMB:
      Sounds->PlaySoundEffect("drop_bomb", _owner->GetPosition());
      break;
    case AmmoType::KINETIC:
    case AmmoType::PLASMA:
      Sounds->PlaySoundEffect("weapon", _owner->GetPosition());
      break;
    }
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

  if(_consumes_ammo)
    {
      assert(_ammo > 0);
      _ammo--;
    }

  return projectile;
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


void Weapon::SetAmmoType(AmmoType type)
{
  _ammotype = type;
  switch(type)
    {
    case AmmoType::KINETIC:
      {
        _projectile_initial_velocity = 10;
        _projectile_damage = 34;
        _projectile_mesh = AssetLoader->LoadMesh("Projectile");
        _minimum_firing_interval = 0.02;
        _firing_heat = 0.03;
        _projectile_use_gravity = true;
      }
      break;
    case AmmoType::PLASMA:
      {
        _projectile_initial_velocity = 20;
        _projectile_damage = 100;
        _projectile_mesh = AssetLoader->LoadMesh("ProjectilePlasma");
        _minimum_firing_interval = 0.09;
        _firing_heat = 0.1;
        _projectile_use_gravity = false;
      }
      break;
    case AmmoType::BOMB:
      {
        _projectile_initial_velocity = 0;
        _projectile_damage           = 1000;
        _projectile_mesh             = AssetLoader->LoadMesh("Bomb");
        _minimum_firing_interval     = 0.25;
        _firing_heat                 = 0.0;
        _projectile_use_gravity      = true;
        _consumes_ammo               = true;
      }
      break;
    }
}


Weapon::AmmoType Weapon::GetAmmoType() const
{
  return _ammotype;
}


unsigned int Weapon::GetWeaponGroup() const
{
  return _weapon_group;
}


void Weapon::SetWeaponGroup(unsigned int group)
{
  _weapon_group = group;
}


unsigned int Weapon::GetAmmoAmount() const
{
  return _ammo;
}


void Weapon::AddAmmoAmount(unsigned int amount)
{
  _ammo = std::min(_ammo + amount, _ammo_max);
}


