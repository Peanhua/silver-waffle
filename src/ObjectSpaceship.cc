/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "Mesh.hh"
#include "NavigationMap.hh"
#include "ObjectLevelEntrance.hh"
#include "ObjectBuilding.hh"
#include "ObjectCollectible.hh"
#include "ObjectPlanet.hh"
#include "ObjectProjectile.hh"
#include "ScreenLevelPlanet.hh"
#include "ShaderProgram.hh"
#include "SpaceshipControlProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemScreen.hh"
#include "SubsystemSfx.hh"
#include "Weapon.hh"
#include <iostream>


ObjectSpaceship::ObjectSpaceship(Scene * scene, unsigned int random_seed)
  : ObjectMovable(scene, random_seed),
    _landed(false),
    _human_count(0),
    _on_human_count_changed(nullptr),
    _human_saving_timer(0),
    _systemlog_enabled(false),
    _has_engine_sound(false),
    _engine_sfx_timer(0),
    _engine_auto_slow_down_axis { true, true, true }
{
  _weapongroups.emplace_back();
  _weapongroups.emplace_back();
  
  SetIsAffectedByGravity(false);

  std::vector<SpaceshipUpgrade::Type> types
    {
      SpaceshipUpgrade::Type::BONUS_DAMAGE,
      SpaceshipUpgrade::Type::SHIELD,
      SpaceshipUpgrade::Type::WEAPON,
      SpaceshipUpgrade::Type::WEAPON_COOLER,
      SpaceshipUpgrade::Type::ENGINE_UPGRADE,
      SpaceshipUpgrade::Type::HULL_UPGRADE,
      SpaceshipUpgrade::Type::REPAIR_DROID,
      SpaceshipUpgrade::Type::WARP_ENGINE,
      SpaceshipUpgrade::Type::PLANET_LANDER,
      SpaceshipUpgrade::Type::BOMB_BAY,
    };
  for(auto t : types)
    {
      auto u = new SpaceshipUpgrade(this, t);
      _upgrades.push_back(u);
    }
  SetTickingRequiresPlayerVisibility(true);
}


void ObjectSpaceship::Tick(double deltatime)
{
  assert(IsAlive());
  for(unsigned int i = 0; i < _control_programs.size(); i++)
    if(_control_programs[i])
      _control_programs[i] = _control_programs[i]->Tick(deltatime);

  bool firing = false;
  for(unsigned int group = 0; !firing && group < _weapongroups.size(); group++)
    for(auto weapon : GetWeapons(group))
      if(weapon->IsAutofireOn())
        {
          firing = true;
          break;
        }
  
  bool engines_on = false;
  for(auto engine : _engines)
    {
      if(engine->_enabled && engine->_throttle > 0.01)
        {
          engines_on = true;
          auto orientation = glm::toMat4(GetOrientation());
          auto direction = (glm::vec4(engine->_thrust_direction, 1) * orientation).xyz();

          if(glm::dot(GetVelocity(), glm::normalize(direction)) < engine->_speed_limit)
            {
              auto power = static_cast<float>(engine->_power * engine->_throttle * deltatime);
              if(firing)
                power *= 0.5f;
              AddImpulse(direction * power);
            }
        }
    }
  if(_has_engine_sound && engines_on)
    {
      _engine_sfx_timer -= deltatime;
      if(_engine_sfx_timer < 0)
        {
          _engine_sfx_timer = 0.1;
          Sounds->PlaySoundEffect("engine", GetPosition());
        }
    }
  else
    _engine_sfx_timer = 0;
      
  if(_engines.size() > 0 && !engines_on)
    { // todo: separate controls and use the engines to slow down
      auto engineup = GetUpgrade(SpaceshipUpgrade::Type::ENGINE_UPGRADE);
      double engine_strength = 0.5 + 1.5 * static_cast<double>(engineup->GetInstallCount()) / static_cast<double>(engineup->GetMaxInstalls());
      if(firing)
        engine_strength *= 0.5;

      bool setvel = false;
      auto vel = GetVelocity();
      bool addimp = false;
      glm::vec3 imp(0);
      for(int i = 0; i < 3; i++)
        if(_engine_auto_slow_down_axis[i])
          {
            if(std::abs(vel[i]) < 0.2f)
              {
                vel[i] = 0;
                setvel = true;
              }
            else if(vel[i] < 0.0f)
              {
                imp[i] = static_cast<float>(engine_strength *  20.0 * deltatime);
                addimp = true;
              }
            else if(vel[i] > 0.0f)
              {
                imp[i] = static_cast<float>(engine_strength * -20.0 * deltatime);
                addimp = true;
              }
          }
      if(setvel)
        SetVelocity(vel);
      if(addimp)
        AddImpulse(imp);
    }

  int weaponcoolercount = GetUpgrade(SpaceshipUpgrade::Type::WEAPON_COOLER)->GetInstallCount();
  for(unsigned int group = 0; group < _weapongroups.size(); group++)
    for(auto weapon : GetWeapons(group))
      {
        if(weapon->IsAutofireOn())
          if(weapon->CanFire())
            weapon->Fire();

        double cooling = 1;
        if(weaponcoolercount > 0)
          {
            cooling += 0.5;
            weaponcoolercount--;
          }

        weapon->Tick(deltatime, cooling);
      }

  for(auto u : _upgrades)
    u->Tick(deltatime);

  SaveHuman(deltatime);

  ObjectMovable::Tick(deltatime);
}


void ObjectSpaceship::AddEngine(const glm::vec3 & thrust_direction, double power)
{
  auto engine = new Engine();
  engine->_thrust_direction = thrust_direction;
  engine->_power = power;
  engine->_throttle = 0.0;
  engine->_enabled = true;
  engine->_speed_limit = 15;
  _engines.push_back(engine);
}


void ObjectSpaceship::EnableEngine(unsigned int engine_id, bool enabled)
{
  assert(engine_id < _engines.size());
  _engines[engine_id]->_enabled = enabled;
}


void ObjectSpaceship::EnableEngines(bool enabled)
{
  for(auto e : _engines)
    e->_enabled = enabled;
}


unsigned int ObjectSpaceship::GetEngineCount() const
{
  return static_cast<unsigned int>(_engines.size());
}


void ObjectSpaceship::AddWeapon()
{
  float id = static_cast<float>((GetWeaponCount(0) + 1) / 2);
  float sign = (GetWeaponCount(0) % 2) == 0 ? 1 : -1;

  AddWeapon(0,
            glm::vec3(sign * id * 0.1f, 1, 0),
            glm::normalize(glm::vec3(sign * id * 0.1f, 1, 0)));
}


void ObjectSpaceship::AddWeaponBomb()
{
  auto weapon = AddWeapon(1, {0, 0, -0.3f}, {0, 0, 0});
  weapon->SetAmmoType(Weapon::AmmoType::BOMB);
  weapon->AddAmmoAmount(6);
}


Weapon * ObjectSpaceship::AddWeapon(unsigned int weapon_group, const glm::vec3 & location, const glm::vec3 & projectile_direction)
{
  assert(weapon_group < _weapongroups.size());
  auto & weapons = _weapongroups[weapon_group];
  auto weapon = new Weapon(this, location, projectile_direction);
  weapon->SetWeaponGroup(weapon_group);
  if(weapons.size() > 0)
    weapon->SetAmmoType(weapons[0]->GetAmmoType());
  weapons.push_back(weapon);
  return weapon;
}


void ObjectSpaceship::RemoveWeapons()
{
  for(auto & group : _weapongroups)
    {
      for(auto weapon : group)
        delete weapon;
      group.clear();
    }
}


unsigned int ObjectSpaceship::GetWeaponCount(unsigned int weapon_group) const
{
  assert(weapon_group < _weapongroups.size());
  return static_cast<unsigned int>(_weapongroups[weapon_group].size());
}


unsigned int ObjectSpaceship::GetWeaponGroupCount() const
{
  return static_cast<unsigned int>(_weapongroups.size());
}


void ObjectSpaceship::SetWeaponGroupAutofire(unsigned int weapon_group, bool enabled)
{
  for(auto weapon : GetWeapons(weapon_group))
    weapon->SetAutofire(enabled);
}


void ObjectSpaceship::EnableWeapons(bool enabled)
{
  // todo
  assert(enabled == enabled);
}



void ObjectSpaceship::SetEngineThrottle(unsigned int engine_id, double throttle)
{
  assert(engine_id < _engines.size());
  _engines[engine_id]->_throttle = throttle;
}


double ObjectSpaceship::GetEngineThrottle(unsigned int engine_id)
{
  assert(engine_id < _engines.size());
  return _engines[engine_id]->_throttle;
}


void ObjectSpaceship::UpgradeEngines(double power_multiplier)
{
  for(auto e : _engines)
    e->_power *= power_multiplier;
}


void ObjectSpaceship::OnHit(Object * perpetrator, Object * hitter, double damage, const glm::vec3 & hit_position, const glm::vec3 & impulse)
{
  if(GetUseHealth())
    {
      std::string syslog("Hit!");
      auto shield = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
      if(shield->GetValue() > 0)
        {
          double reduction = std::min(shield->GetValue(), damage);
          
          shield->AdjustValue(-reduction);
          
          damage -= reduction;
          
          if(shield->GetValue() > 0)
            syslog += " Shields hold!";
          else
            syslog += " Shields down!";
        }
      SystemlogAppend(syslog + " Hull damage: " + std::to_string(std::lround(damage)) + "\n");
    }
  ObjectMovable::OnHit(perpetrator, hitter, damage, hit_position, impulse);
}


SpaceshipUpgrade * ObjectSpaceship::GetUpgrade(SpaceshipUpgrade::Type type) const
{
  for(auto u : _upgrades)
    if(u->GetType() == type)
      return u;
  assert(false);
  return nullptr;
}


double ObjectSpaceship::GetGlow() const
{
  auto shield = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
  if(shield->IsActive())
    return shield->GetValue() / GetMaxHealth();
  return 0;
}


void ObjectSpaceship::UpgradeFromCollectible(ObjectCollectible * collectible)
{
  for(auto u : _upgrades)
    u->ActivateFromCollectible(collectible);
}




void ObjectSpaceship::CopyUpgrades(const ObjectSpaceship & source)
{
  _engines.clear();
  for(auto e : source._engines)
    _engines.push_back(new Engine(*e));

  for(unsigned int group = 0; group < 2; group++)
    {
      _weapongroups[group].clear();
      for(auto w : source._weapongroups[group])
        {
          auto nw = new Weapon(*w);
          _weapongroups[group].push_back(nw);
          nw->SetOwner(this);
        }
    }

  _upgrades.clear();
  for(auto u : source._upgrades)
    {
      auto nu = new SpaceshipUpgrade(*u);
      nu->SetOwner(this);
      _upgrades.push_back(nu);
    }
}


void ObjectSpaceship::ClearControlPrograms()
{
  _control_programs.clear();
}


void ObjectSpaceship::AddControlProgram(SpaceshipControlProgram * program)
{
  _control_programs.push_back(program);
}


unsigned int ObjectSpaceship::GetActiveControlProgramCount() const
{
  unsigned int count = 0;
  for(auto p : _control_programs)
    if(p)
      count++;
  
  return count;
}




void ObjectSpaceship::AddNamedControlProgram(const std::string & name)
{
  if(name == "test1")
    {
      SpaceshipControlProgram * p = new SCP_MoveForward(this, 2, 5);
      AddControlProgram(p);

      for(int i = 0; i < 10; i++)
        {
          SpaceshipControlProgram * pp = new SCP_Delay(this, 5);
          p->SetNext(pp);

          p = new SCP_MoveForward(this, 2, 5);
          pp->SetNext(p);
        }


      p = new SCP_ChanceToFire(this, 0.02, 9999);
      AddControlProgram(p);
    }
  else if(name == "test2")
    {
      SpaceshipControlProgram * p = new SCP_MoveForward(this, 2, 500);
      AddControlProgram(p);
      p = new SCP_MoveSidewaysSin(this, 1.0, 5.0, 9999);
      AddControlProgram(p);
    }
  else if(name == "passive-forward")
    {
      AddControlProgram(new SCP_MoveForward(this, 2, 999));
    }
  else if(name == "forward")
    {
      AddControlProgram(new SCP_MoveForward(this, 2, 999));
      AddControlProgram(new SCP_ChanceToFire(this, 0.02, 999));
    }
  else if(name == "snake-forward")
    {
      AddControlProgram(new SCP_MoveForward(this, 2, 999));
      AddControlProgram(new SCP_MoveSidewaysSin(this, 1.5, 1.5, 999));
      AddControlProgram(new SCP_ChanceToFire(this, 0.02, 999));
    }
  else if(name == "erratic-forward")
    {
      SpaceshipControlProgram * p = new SCP_MoveForward(this, 1, 999);
      AddControlProgram(p);

      p = new SCP_MoveForward(this, 1, 1);
      AddControlProgram(p);
      for(int i = 0; i < 100; i++)
        {
          SpaceshipControlProgram * pp = new SCP_MoveRight(this, 1, 1);
          p->SetNext(pp);
          p = pp;
          
          pp = new SCP_MoveRight(this, -2, 1);
          p->SetNext(pp);
          p = pp;
          
          pp = new SCP_MoveRight(this, 1, 1);
          p->SetNext(pp);
          p = pp;
        }

      AddControlProgram(new SCP_ChanceToFire(this, 0.02, 999));
    }
  else if(name == "stepping-forward")
    {
      SpaceshipControlProgram * p = new SCP_MoveForward(this, 2, 5);
      AddControlProgram(p);

      for(int i = 0; i < 10; i++)
        {
          SpaceshipControlProgram * pp = new SCP_Delay(this, 5);
          p->SetNext(pp);

          p = new SCP_MoveForward(this, 2, 5);
          pp->SetNext(p);
        }

      AddControlProgram(new SCP_ChanceToFire(this, 0.02, 999));
    }
  else if(name == "snake-stepping-forward")
    {
      SpaceshipControlProgram * p = new SCP_MoveForward(this, 2, 5);
      AddControlProgram(p);

      for(int i = 0; i < 10; i++)
        {
          SpaceshipControlProgram * pp = new SCP_Delay(this, 5);
          p->SetNext(pp);

          p = new SCP_MoveForward(this, 2, 5);
          pp->SetNext(p);
        }
      AddControlProgram(new SCP_MoveSidewaysSin(this, 1.0, 5.0, 999));

      AddControlProgram(new SCP_ChanceToFire(this, 0.02, 999));
    }
  else if(name == "boss")
    {
      AddControlProgram(new SCP_MoveForward(this, 5, 5));
      AddControlProgram(new SCP_ChanceToFire(this, 0.02, 999));
    }
  else if(name == "wait10s")
    {
      AddControlProgram(new SCP_Delay(this, 10));
    }
  else if(name == "waithoriz5-shoot")
    {
      AddControlProgram(new SCP_MoveForward(this, 2, 999));

      SpaceshipControlProgram * p, * pp;

      p = new SCP_Infinitely(this);
      AddControlProgram(p);

      pp = new SCP_WaitForPlayerHorizontalDistanceMin(this, 5);
      p->SetNext(pp);

      p = pp;
      pp = new SCP_ChanceToFire(this, 0.03, 1);
      p->SetNext(pp);
    }
  else if(name == "wait15-shoot")
    {
      AddControlProgram(new SCP_MoveForward(this, 2, 999));

      SpaceshipControlProgram * p, * pp;

      p = new SCP_Infinitely(this);
      AddControlProgram(p);

      pp = new SCP_WaitForPlayerDistanceMin(this, 15);
      p->SetNext(pp);

      p = pp;
      pp = new SCP_ChanceToFire(this, 0.03, 1);
      p->SetNext(pp);
    }
  else if(name == "face-player-shoot")
    {
      SpaceshipControlProgram * p, * pp;

      p = new SCP_Infinitely(this);
      AddControlProgram(p);

      pp = new SCP_WaitForPlayerDistanceMin(this, 15);
      p->SetNext(pp);

      p = pp;
      pp = new SCP_FacePlayer(this);
      p->SetNext(pp);

      p = pp;
      pp = new SCP_ChanceToFire(this, 0.10, 1);
      p->SetNext(pp);
    }
  else if(name == "negative-y")
    {
      auto p = new SCP_Infinitely(this);
      AddControlProgram(p);
      
      auto pp = new SCP_MoveTowards(this, {0, -1, 0}, 999);
      p->SetNext(pp);
    }
  else if(name == "pathfind-to-somewhere")
    {
      auto destination = glm::vec3(GetScene()->GetNavigationMap()->NavigationToWorld(glm::ivec2(56, 17)), 0);
      destination.z = destination.y;
      destination.y = 0;

      auto p = new SCP_PathMoveTo(this, destination);
      AddControlProgram(p);
    }
  else if(name == "follow-player")
    {
      SpaceshipControlProgram * p, * pp;
      
      p = new SCP_FollowPlayer(this);
      AddControlProgram(p);

      p = new SCP_Infinitely(this);
      AddControlProgram(p);
      pp = new SCP_WaitForPlayerDistanceMin(this, 15);
      p->SetNext(pp);
      p = pp;
      pp = new SCP_FacePlayer(this);
      p->SetNext(pp);
      p = pp;
      pp = new SCP_ChanceToFire(this, 0.10, 1);
      p->SetNext(pp);
    }
  else
    assert(false);
}


void ObjectSpaceship::SystemlogAppend(const std::string & message)
{
  if(_systemlog_enabled)
    {
      std::cout << message;
      _systemlog += message;
    }
}


const std::string & ObjectSpaceship::SystemlogGet() const
{
  return _systemlog;
}


void ObjectSpaceship::SystemlogClear()
{
  if(_systemlog_enabled)
    _systemlog.clear();
}


void ObjectSpaceship::SystemlogEnable()
{
  _systemlog_enabled = true;
}


void ObjectSpaceship::AddHuman()
{
  _human_count++;
  if(_on_human_count_changed)
    _on_human_count_changed();

  auto gs = GetOwnerGameStats();
  if(gs)
    gs->OnHumanCollected();
}


int ObjectSpaceship::GetHumanCount() const
{
  return _human_count;
}


void ObjectSpaceship::SaveHuman(double deltatime)
{
  if(!_landed)
    return;

  if(_human_count == 0)
    return;

  _human_saving_timer += deltatime;
  if(_human_saving_timer < 1)
    return;
  _human_saving_timer -= 1;
  
  Sounds->PlaySoundEffect("human_jump", GetPosition());
  SystemlogAppend("You rescued another human individual!\n");
  auto gs = GetOwnerGameStats();
  if(gs)
    {
      gs->AddScore(10);
      gs->OnHumansSaved(1);
    }
  
  _human_count--;
  
  if(_on_human_count_changed)
    _on_human_count_changed();
}


void ObjectSpaceship::SetOnHumanCountChanged(on_human_count_changed_t callback)
{
  if(callback)
    assert(!_on_human_count_changed);
  else
    assert(_on_human_count_changed);
  _on_human_count_changed = callback;
}


void ObjectSpaceship::LandOnSpaceport(ObjectBuilding * spaceport)
{
  assert(spaceport);
  SystemlogAppend("Landing on nearby spaceport.\n");
  _landed = true;
  _human_saving_timer = 0;
  
  auto target = spaceport->GetPosition();
  target.z += spaceport->GetMesh()->GetBoundingBoxHalfSize().z;
  target.z += GetMesh()->GetBoundingBoxHalfSize().z;
  auto p = new SCP_MoveTo(this, target, 5, false);
  AddControlProgram(p);
  SetUseHealth(false);
  SetVelocity({0, 0, 0});
  EnableEngines(false);
  EnableWeapons(false);
}


void ObjectSpaceship::LaunchFromSpaceport()
{
  SystemlogAppend("Launch!\n");
  _landed = false;
  SetUseHealth(true);
  AddImpulse({0, 0, 10});
  EnableEngines(true);
  EnableWeapons(true);
}


void ObjectSpaceship::DescendToPlanet(SolarSystemObject * planet)
{
  auto current = dynamic_cast<ScreenLevel *>(ScreenManager->GetScreen());
  assert(current);

  auto ns = new ScreenLevelPlanet(current, planet);
  ns->SetupLevels();
  current->TransitionToScreen(ns, "Entering " + planet->GetName() + "\n");
}


void ObjectSpaceship::LaunchToSpace()
{
  LaunchFromSpaceport();
  SetVelocity({0, 0, 0});

  auto targetpos = glm::vec3(GetPosition().xy(), GetScene()->GetPlayAreaSize().z * 0.5f);
  targetpos.z -= static_cast<float>(GetMesh()->GetBoundingSphereRadius());
  
  auto p = new SCP_MoveTo(this, targetpos, 20, true);
  AddControlProgram(p);

  auto pp = new SCP_ExitCurrentLevel(this);
  p->SetNext(pp);
}


bool ObjectSpaceship::IsLanded() const
{
  return _landed;
}


void ObjectSpaceship::OnDestroyed(Object * destroyer)
{
  Sounds->PlaySoundEffect("explosion.small", GetPosition());
  
  for(auto p : _projectiles_fired)
    if(p)
      p->SetOwner(nullptr);
  _projectiles_fired.clear();
  
  ObjectMovable::OnDestroyed(destroyer);
}


void ObjectSpaceship::OnFiredProjectileDestroyed(ObjectProjectile * projectile)
{
  assert(projectile);
  for(unsigned int i = 0; i < _projectiles_fired.size(); i++)
    if(_projectiles_fired[i] == projectile)
      {
        _projectiles_fired[i] = nullptr;
        break;
      }
}


const std::vector<Weapon *> & ObjectSpaceship::GetWeapons(unsigned int weapon_group) const
{
  assert(weapon_group < _weapongroups.size());
  return _weapongroups[weapon_group];
}


void ObjectSpaceship::SetEngineSound(bool enabled)
{
  _has_engine_sound = enabled;
}


void ObjectSpaceship::SetEngineAutoSlowDownAxis(int axis, bool enabled)
{
  assert(axis >= 0);
  assert(axis <= 2);
  _engine_auto_slow_down_axis[axis] = enabled;
}

