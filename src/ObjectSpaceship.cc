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
#include "ObjectLevelEntrance.hh"
#include "ObjectBuilding.hh"
#include "ObjectCollectible.hh"
#include "ObjectPlanet.hh"
#include "ScreenLevelPlanet.hh"
#include "ShaderProgram.hh"
#include "SpaceshipControlProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemScreen.hh"
#include "Weapon.hh"
#include <iostream>


ObjectSpaceship::ObjectSpaceship(Scene * scene, unsigned int random_seed)
  : ObjectMovable(scene, random_seed),
    _gamestats(nullptr),
    _landed(false),
    _human_count(0),
    _on_human_count_changed(nullptr),
    _human_saving_timer(0),
    _systemlog_enabled(false)
{
  SetIsAffectedByGravity(false);

  std::vector<SpaceshipUpgrade::Type> types
    {
      SpaceshipUpgrade::Type::BONUS_DAMAGE,
      SpaceshipUpgrade::Type::SHIELD,
      SpaceshipUpgrade::Type::WEAPON,
      SpaceshipUpgrade::Type::WEAPON_COOLER,
      SpaceshipUpgrade::Type::ENGINE_UPGRADE,
      SpaceshipUpgrade::Type::HULL_UPGRADE,
      SpaceshipUpgrade::Type::EVASION_MANEUVER,
      SpaceshipUpgrade::Type::REPAIR_DROID,
      SpaceshipUpgrade::Type::WARP_ENGINE,
      SpaceshipUpgrade::Type::PLANET_LANDER,
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
  for(unsigned int i = 0; !firing && i < _weapons.size(); i++)
    if(_weapons[i]->IsAutofireOn())
      firing = true;
  
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

  auto weaponcoolercount = static_cast<unsigned int>(GetUpgrade(SpaceshipUpgrade::Type::WEAPON_COOLER)->GetInstallCount());
  for(unsigned int i = 0; i < _weapons.size(); i++)
    {
      auto weapon = _weapons[i];
      
      if(weapon->IsAutofireOn())
        FireWeapon(i);

      double cooling = 1;
      if(i < weaponcoolercount)
        cooling += 0.5;

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
  float id = static_cast<float>((GetWeaponCount() + 1) / 2);
  float sign = (GetWeaponCount() % 2) == 0 ? 1 : -1;

  AddWeapon(glm::vec3(sign * id * 0.1f, 1, 0),
            glm::normalize(glm::vec3(sign * id * 0.1f, 1, 0)));
}


void ObjectSpaceship::AddWeapon(const glm::vec3 & location, const glm::vec3 & projectile_direction)
{
  auto weapon = new Weapon(this, location, projectile_direction);
  if(_weapons.size() > 0)
    weapon->SetAmmo(_weapons[0]->GetAmmo());
  _weapons.push_back(weapon);
}


void ObjectSpaceship::RemoveWeapons()
{
  for(auto w : _weapons)
    delete w;
  _weapons.clear();
}


Weapon * ObjectSpaceship::GetWeapon(unsigned int weapon_id) const
{
  assert(weapon_id < _weapons.size());
  return _weapons[weapon_id];
}


unsigned int ObjectSpaceship::GetWeaponCount() const
{
  assert(_weapons.size() < 0xffff);
  return static_cast<unsigned int>(_weapons.size());
}


void ObjectSpaceship::FireWeapon(unsigned int weapon_id)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  if(!weapon->CanFire())
    return;
  
  weapon->Fire();
}


void ObjectSpaceship::SetWeaponAutofire(unsigned int weapon_id, bool enabled)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  weapon->SetAutofire(enabled);
}


double ObjectSpaceship::GetWeaponHeat(unsigned int weapon_id) const
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  return weapon->GetHeat();
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


void ObjectSpaceship::Hit(Object * perpetrator, double damage, const glm::vec3 & impulse)
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
  ObjectMovable::Hit(perpetrator, damage, impulse);
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




void ObjectSpaceship::SetOwnerGameStats(GameStats * gamestats)
{
  _gamestats = gamestats;
}


GameStats * ObjectSpaceship::GetOwnerGameStats() const
{
  return _gamestats;
}


void ObjectSpaceship::CopyUpgrades(const ObjectSpaceship & source)
{
  _engines.clear();
  for(auto e : source._engines)
    _engines.push_back(new Engine(*e));

  _weapons.clear();
  for(auto w : source._weapons)
    _weapons.push_back(new Weapon(*w));
  for(auto w : _weapons)
    w->SetOwner(this);

  _upgrades.clear();
  for(auto u : source._upgrades)
    {
      auto nu = new SpaceshipUpgrade(*u);
      nu->SetOwner(this);
      _upgrades.push_back(nu);
    }
}


uint64_t ObjectSpaceship::GetCollidesWithChannels() const
{
  bool evading = GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER)->GetTimer() > 0.0;

  uint64_t rv = 0;
  if(!evading)
    rv = ObjectMovable::GetCollidesWithChannels();

  return rv;
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
  if(_gamestats)
    _gamestats->OnHumanCollected();
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
  
  SystemlogAppend("You rescued another human individual!\n");
  if(_gamestats)
    {
      _gamestats->AddScore(10);
      _gamestats->OnHumansSaved(1);
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
  current->TransitionToScreen(ns, "Descending to " + planet->GetName() + "\n");
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
