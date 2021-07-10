#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "Mesh.hh"
#include "ObjectBonusLevelEntrance.hh"
#include "ObjectCollectible.hh"
#include "ShaderProgram.hh"
#include "SpaceshipControlProgram.hh"
#include "SubsystemAssetLoader.hh"


ObjectSpaceship::ObjectSpaceship(Scene * scene)
  : ObjectMovable(scene),
    _gamestats(nullptr),
    _systemlog_enabled(false)
{
  AddCollidesWithChannel(Object::CollisionChannel::PROJECTILE);

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
}


void ObjectSpaceship::Tick(double deltatime)
{
  ObjectMovable::Tick(deltatime);

  for(unsigned int i = 0; i < _control_programs.size(); i++)
    if(_control_programs[i])
      _control_programs[i] = _control_programs[i]->Tick(deltatime);
  
  bool engines_on = false;
  for(auto engine : _engines)
    {
      if(engine->_enabled && engine->_throttle > 0.01)
        {
          engines_on = true;
          auto orientation = glm::toMat4(GetOrientation());
          auto direction = glm::vec4(engine->_thrust_direction, 1) * orientation;
          auto power = static_cast<float>(engine->_power * engine->_throttle * deltatime);
          AddImpulse(direction.xyz() * power);
        }
    }
      
  if(_engines.size() > 0 && !engines_on)
    { // todo: separate controls and use the engines to slow down
      auto engineup = GetUpgrade(SpaceshipUpgrade::Type::ENGINE_UPGRADE);
      double engine_strength = 0.5 + 1.5 * static_cast<double>(engineup->GetInstallCount()) / static_cast<double>(engineup->GetMaxInstalls());

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

  int weaponcoolercount = GetUpgrade(SpaceshipUpgrade::Type::WEAPON_COOLER)->GetInstallCount();
  for(unsigned int i = 0; i < _weapons.size(); i++)
    {
      auto weapon = _weapons[i];
      
      if(weapon->_autofire)
        FireWeapon(i);

      double cooling = 0.1;
      if(static_cast<int>(i) < weaponcoolercount)
        cooling += 0.05;
      
      weapon->_heat = std::max(0.0, weapon->_heat - cooling * deltatime);

      if(weapon->_last_fire_timer > 0.0)
        weapon->_last_fire_timer -= deltatime;
    }

  for(auto u : _upgrades)
    u->Tick(deltatime);
}


void ObjectSpaceship::AddEngine(const glm::vec3 & thrust_direction, double power)
{
  auto engine = new Engine();
  engine->_thrust_direction = thrust_direction;
  engine->_power = power;
  engine->_throttle = 0.0;
  engine->_enabled = true;
  _engines.push_back(engine);
}


void ObjectSpaceship::EnableEngine(unsigned int engine_id, bool enabled)
{
  assert(engine_id < _engines.size());
  _engines[engine_id]->_enabled = enabled;
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
            AssetLoader->LoadMesh("Projectile"),
            glm::normalize(glm::vec3(sign * id * 0.1f, 1, 0)),
            10.0,
            34.0);
}


void ObjectSpaceship::AddWeapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage)
{
  auto weapon = new Weapon(location, projectile, projectile_direction, projectile_initial_velocity, projectile_damage);
  _weapons.push_back(weapon);
}


void ObjectSpaceship::RemoveWeapons()
{
  for(auto w : _weapons)
    delete w;
  _weapons.clear();
}


  
unsigned int ObjectSpaceship::GetWeaponCount() const
{
  assert(_weapons.size() < 0xffff);
  return static_cast<unsigned int>(_weapons.size());
}


bool ObjectSpaceship::FireWeapon(unsigned int weapon_id)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];

  if(weapon->_last_fire_timer > 0.0)
    return false;
  
  if(weapon->_heat > 1.0)
    return false;

  auto em = GetScene()->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER);
  if(em->IsActive())
    return false;
  
  auto damage = weapon->_projectile_damage;
  auto bonus = GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE);
  if(bonus->IsActive())
    damage *= bonus->GetValue();

  auto orientation = glm::toMat4(glm::inverse(GetOrientation()));
  auto location = glm::vec4(weapon->_location, 1) * orientation;
  auto direction = glm::vec4(weapon->_projectile_direction, 1) * orientation;
  GetScene()->AddProjectile(this,
                            GetPosition() + location.xyz(),
                            GetVelocity() * 0.5f + direction.xyz() * static_cast<float>(weapon->_projectile_initial_velocity),
                            damage,
                            10.0);
  weapon->_heat += 0.03;
  weapon->_last_fire_timer = weapon->_minimum_firing_interval;
  
  return true;
}


void ObjectSpaceship::SetWeaponAutofire(unsigned int weapon_id, bool enabled)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  weapon->_autofire = enabled;
}


double ObjectSpaceship::GetWeaponHeat(unsigned int weapon_id) const
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  return weapon->_heat;
}


void ObjectSpaceship::SetEngineThrottle(unsigned int engine_id, double throttle)
{
  assert(engine_id < _engines.size());
  _engines[engine_id]->_throttle = throttle;
}


void ObjectSpaceship::UpgradeEngines(double power_multiplier)
{
  for(auto e : _engines)
    e->_power *= power_multiplier;
}


void ObjectSpaceship::Hit(Object * perpetrator, double damage, const glm::vec3 & impulse)
{
  std::string syslog("Hit!");
  auto shield = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
  if(shield->IsActive())
    {
      double reduction = std::min(shield->GetValue(), damage);
      
      shield->AdjustValue(-reduction);
     
      damage -= reduction;

      if(shield->IsActive())
        syslog += " Shields hold!";
      else
        syslog += " Shields down!";
    }
  SystemlogAppend(syslog + " Hull damage: " + std::to_string(std::lround(damage)) + "\n");
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


void ObjectSpaceship::Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const
{
  auto mesh = GetMesh();
  if(mesh)
    {
      const glm::mat4 model(glm::translate(glm::mat4(1), GetPosition()) * glm::toMat4(GetOrientation()));
      const glm::mat4 mvp(vp * model);

      auto shader = mesh->GetShaderProgram();
      shader->Use();
      auto shield = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
      shader->SetVec("in_glow", shield->IsActive() ? glm::vec3(0.5, 0.5, 1.0) * static_cast<float>(shield->GetValue() / GetMaxHealth()) : glm::vec3(0.0, 0.0, 0.0));

      mesh->Draw(model, view, projection, mvp, shader);
    }
}


void ObjectSpaceship::UpgradeFromCollectible(ObjectCollectible * collectible)
{
  for(auto u : _upgrades)
    u->ActivateFromCollectible(collectible);
}


ObjectSpaceship::Weapon::Weapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage)
  : _location(location),
    _autofire(false),
    _heat(0),
    _last_fire_timer(0),
    _minimum_firing_interval(0.02),
    _projectile(projectile),
    _projectile_direction(projectile_direction),
    _projectile_initial_velocity(projectile_initial_velocity),
    _projectile_damage(projectile_damage)
{
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

  _upgrades.clear();
  for(auto u : source._upgrades)
    _upgrades.push_back(new SpaceshipUpgrade(*u));
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


      p = new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 9999);
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
      AddControlProgram(new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 999));
    }
  else if(name == "snake-forward")
    {
      AddControlProgram(new SCP_MoveForward(this, 2, 999));
      AddControlProgram(new SCP_MoveSidewaysSin(this, 1.5, 1.5, 999));
      AddControlProgram(new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 999));
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

      AddControlProgram(new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 999));
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

      AddControlProgram(new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 999));
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

      AddControlProgram(new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 999));
    }
  else if(name == "boss")
    {
      AddControlProgram(new SCP_MoveForward(this, 5, 5));
      AddControlProgram(new SCP_ChanceToFire(this, reinterpret_cast<unsigned long>(this), 0.02, 999));
    }
  else if(name == "wait10s")
    {
      AddControlProgram(new SCP_Delay(this, 10));
    }
  else
    assert(false);
}


void ObjectSpaceship::SystemlogAppend(const std::string & message)
{
  if(_systemlog_enabled)
    _systemlog += message;
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
