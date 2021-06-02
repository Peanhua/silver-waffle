#include "ObjectSpaceship.hh"
#include "Scene.hh"


ObjectSpaceship::ObjectSpaceship(Scene * scene)
  : ObjectMovable(scene),
    _bonus_damage_multiplier(1),
    _bonus_damage_timer(0),
    _shield(0),
    _shield_timer(0)
{
}


void ObjectSpaceship::Tick(double deltatime)
{
  ObjectMovable::Tick(deltatime);

  bool engines_on = false;
  for(auto engine : _engines)
    if(engine->_throttle > 0.01)
      {
        engines_on = true;
        AddImpulse(engine->_thrust_direction * static_cast<float>(engine->_power * engine->_throttle * deltatime));
      }

  if(_engines.size() > 0 && !engines_on)
    { // todo: separate controls and use the engines to slow down
      const auto vel = GetVelocity();
      if(std::abs(vel.x) < 0.1f)
        SetVelocity(glm::vec3(0, vel.y, vel.z));
      else if(vel.x < 0.0f)
        AddImpulse(glm::vec3(5.0 * deltatime, 0, 0));
      else if(vel.x > 0.0f)
        AddImpulse(glm::vec3(-5.0 * deltatime, 0, 0));
    }

  for(unsigned int i = 0; i < _weapons.size(); i++)
    {
      auto weapon = _weapons[i];
      
      if(weapon->_autofire)
        FireWeapon(i);
      
      weapon->_heat = std::max(0.0, weapon->_heat - 0.1 * deltatime);
    }

  if(_bonus_damage_timer > 0.0)
    _bonus_damage_timer -= deltatime;
  if(_shield_timer > 0.0)
    _shield_timer -= deltatime;
}


unsigned int ObjectSpaceship::AddEngine(const glm::vec3 & thrust_direction, double power)
{
  auto engine = new Engine();
  engine->_thrust_direction = thrust_direction;
  engine->_power = power;
  engine->_throttle = 0.0;
  _engines.push_back(engine);
  return static_cast<unsigned int>(_engines.size());
}


unsigned int ObjectSpaceship::AddWeapon(const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage)
{
  auto weapon = new Weapon();
  weapon->_location = location;
  weapon->_autofire = false;
  weapon->_projectile = projectile;
  weapon->_projectile_direction = projectile_direction;
  weapon->_projectile_initial_velocity = projectile_initial_velocity;
  weapon->_projectile_damage = projectile_damage;
  _weapons.push_back(weapon);
  return static_cast<unsigned int>(_weapons.size());
}

  
bool ObjectSpaceship::FireWeapon(unsigned int weapon_id)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  if(weapon->_heat > 1.0)
    return false;

  auto damage = weapon->_projectile_damage;
  if(_bonus_damage_timer > 0.0)
    damage *= _bonus_damage_multiplier;
  
  _scene->AddProjectile(this,
                        GetPosition() + weapon->_location,
                        GetVelocity() * 0.5f + weapon->_projectile_direction * static_cast<float>(weapon->_projectile_initial_velocity),
                        damage,
                        10.0);
  weapon->_heat += 0.03;
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


void ObjectSpaceship::SetEnginePower(unsigned int engine_id, double throttle)
{
  assert(engine_id < _engines.size());
  _engines[engine_id]->_throttle = throttle;
}


void ObjectSpaceship::ActivateBonusDamageMultiplier(double multiplier, double time)
{
  _bonus_damage_multiplier = multiplier;
  _bonus_damage_timer = time;
}


double ObjectSpaceship::GetBonusDamageTimer() const
{
  return _bonus_damage_timer;
}


void ObjectSpaceship::Hit(double damage, const glm::vec3 & impulse)
{
  if(_shield_timer > 0.0)
    {
      double reduction = std::min(_shield, damage);
      
      _shield -= reduction;
      if(_shield < 0.0)
        _shield_timer = 0.0;
      
      damage -= reduction;
    }
  ObjectMovable::Hit(damage, impulse);
}


void ObjectSpaceship::ActivateShield(double amount, double time)
{
  _shield = std::max(_shield, amount);
  _shield_timer += time;
}


double ObjectSpaceship::GetShieldTimer() const
{
  return _shield_timer;
}

