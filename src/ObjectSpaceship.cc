#include "ObjectSpaceship.hh"
#include "Scene.hh"

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

  const auto max_x = 10.3f;
  const auto pos = GetPosition();
  const auto vel = GetVelocity();
  if(pos.x < -max_x)
    {
      SetPosition(glm::vec3(-max_x, pos.yz()));
      if(vel.x < 0.0f)
        SetVelocity(glm::vec3(0.0f, vel.yz()));
    }
  else if(pos.x > max_x)
    {
      SetPosition(glm::vec3(max_x, pos.yz()));
      if(vel.x > 0.0f)
        SetVelocity(glm::vec3(0.0f, vel.yz()));
    }
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
  weapon->_projectile = projectile;
  weapon->_projectile_direction = projectile_direction;
  weapon->_projectile_initial_velocity = projectile_initial_velocity;
  weapon->_projectile_damage = projectile_damage;
  _weapons.push_back(weapon);
  return static_cast<unsigned int>(_weapons.size());
}

  
void ObjectSpaceship::FireWeapon(unsigned int weapon_id)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  _scene->AddProjectile(this,
                        GetPosition() + weapon->_location,
                        GetVelocity() * 0.5f + weapon->_projectile_direction * static_cast<float>(weapon->_projectile_initial_velocity),
                        weapon->_projectile_damage,
                        10.0);
}


void ObjectSpaceship::SetEnginePower(unsigned int engine_id, double throttle)
{
  assert(engine_id < _engines.size());
  _engines[engine_id]->_throttle = throttle;
}

