#include "ObjectSpaceship.hh"
#include "Scene.hh"

#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"


ObjectSpaceship::ObjectSpaceship(Scene * scene)
  : ObjectMovable(scene)
{
  {
    auto u = new Upgrade();
    u->_type = Upgrade::Type::BONUS_DAMAGE;
    u->_value = 0;
    u->_timer = 0;
    _upgrades.push_back(u);
  }
  {
    auto u = new Upgrade();
    u->_type = Upgrade::Type::SHIELD;
    u->_value = 0;
    u->_timer = 0;
    _upgrades.push_back(u);
  }
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

  for(auto u : _upgrades)
    u->Tick(deltatime);
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
  auto bonus = GetUpgrade(Upgrade::Type::BONUS_DAMAGE);
  if(bonus->IsActive() > 0.0)
    damage *= bonus->_value;
  
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


void ObjectSpaceship::Hit(double damage, const glm::vec3 & impulse)
{
  auto shield = GetUpgrade(Upgrade::Type::SHIELD);
  if(shield->IsActive())
    {
      double reduction = std::min(shield->_value, damage);
      
      shield->_value -= reduction;
      if(shield->_value < 0.0001)
        shield->_timer = 0.0;
      
      damage -= reduction;
    }
  ObjectMovable::Hit(damage, impulse);
}


ObjectSpaceship::Upgrade * ObjectSpaceship::GetUpgrade(Upgrade::Type type) const
{
  for(auto u : _upgrades)
    if(u->_type == type)
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
      auto shader = AssetLoader->LoadShaderProgram("Spaceship");
      shader->Use();
      auto shield = GetUpgrade(Upgrade::Type::SHIELD);
      shader->SetInt("in_shields", shield->IsActive() ? 1 : 0);
      shader->SetFloat("in_time", static_cast<float>(shield->_timer - std::floor(shield->_timer)));
      mesh->Draw(model, view, projection, mvp, shader);
    }
}


void ObjectSpaceship::Upgrade::Tick(double deltatime)
{
  if(_timer > 0.0)
    _timer -= deltatime;
}


bool ObjectSpaceship::Upgrade::IsActive() const
{
  return _timer > 0.0;
}


void ObjectSpaceship::Upgrade::AddFromCollectible(ObjectCollectible * collectible)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      if(collectible->HasBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER))
        Add(collectible->GetBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER), 30.0);
      break;
    case Type::SHIELD:
      if(collectible->HasBonus(ObjectCollectible::TYPE_SHIELD))
        Add(std::max(_value, collectible->GetBonus(ObjectCollectible::TYPE_DAMAGE_MULTIPLIER)), 30.0);
      break;
    }
}


void ObjectSpaceship::UpgradeFromCollectible(ObjectCollectible * collectible)
{
  for(auto u : _upgrades)
    u->AddFromCollectible(collectible);
}


void ObjectSpaceship::AddUpgrade(Upgrade::Type type, double value, double time)
{
  auto upgrade = GetUpgrade(type);
  upgrade->Add(value, time);
}


void ObjectSpaceship::Upgrade::Add(double amount, double time)
{
  switch(_type)
    {
    case Type::BONUS_DAMAGE:
      _value = amount;
      _timer = time;
      break;
    case Type::SHIELD:
      _value = amount;
      _timer += time;
      break;
    }
}
