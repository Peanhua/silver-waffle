#include "Weapon.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"


Weapon::Weapon(Object * owner, const glm::vec3 & location, Mesh * projectile, const glm::vec3 & projectile_direction, double projectile_initial_velocity, double projectile_damage)
  : _owner(owner),
    _location(location),
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
  spaceship->GetScene()->AddProjectile(spaceship,
                                       spaceship->GetPosition() + location.xyz(),
                                       spaceship->GetVelocity() * 0.5f + direction.xyz() * static_cast<float>(_projectile_initial_velocity),
                                       damage,
                                       5.0);
  _heat += 0.03;
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
