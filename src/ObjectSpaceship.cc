#include "ObjectSpaceship.hh"
#include "Scene.hh"

#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"


ObjectSpaceship::ObjectSpaceship(Scene * scene)
  : ObjectMovable(scene)
{
  std::vector<SpaceshipUpgrade::Type> types
    {
      SpaceshipUpgrade::Type::BONUS_DAMAGE,
      SpaceshipUpgrade::Type::SHIELD,
      SpaceshipUpgrade::Type::WEAPON,
      SpaceshipUpgrade::Type::WEAPON_COOLER,
      SpaceshipUpgrade::Type::ENGINE_UPGRADE,
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

  bool engines_on = false;
  for(auto engine : _engines)
    if(engine->_throttle > 0.01)
      {
        engines_on = true;
        AddImpulse(engine->_thrust_direction * static_cast<float>(engine->_power * engine->_throttle * deltatime));
      }

  if(_engines.size() > 0 && !engines_on)
    { // todo: separate controls and use the engines to slow down
      double engine_strength = 0.25 + 2.75 * static_cast<double>(GetUpgrade(SpaceshipUpgrade::Type::ENGINE_UPGRADE)->GetIntValue()) / 3.0;
      const auto vel = GetVelocity();
      if(std::abs(vel.x) < 0.1f)
        SetVelocity(glm::vec3(0, vel.y, vel.z));
      else if(vel.x < 0.0f)
        AddImpulse(glm::vec3(engine_strength *  20.0 * deltatime, 0, 0));
      else if(vel.x > 0.0f)
        AddImpulse(glm::vec3(engine_strength * -20.0 * deltatime, 0, 0));
    }

  int weaponcoolercount = GetUpgrade(SpaceshipUpgrade::Type::WEAPON_COOLER)->GetIntValue();
  for(unsigned int i = 0; i < _weapons.size(); i++)
    {
      auto weapon = _weapons[i];
      
      if(weapon->_autofire)
        FireWeapon(i);

      double cooling = 0.1;
      if(static_cast<int>(i) < weaponcoolercount)
        cooling += 0.05;
      
      weapon->_heat = std::max(0.0, weapon->_heat - cooling * deltatime);
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


unsigned int ObjectSpaceship::AddWeapon()
{
  float id = static_cast<float>((GetWeaponCount() + 1) / 2);
  float sign = (GetWeaponCount() % 2) == 0 ? 1 : -1;

  return AddWeapon(glm::vec3(sign * id * 0.1f, 1, 0),
                   AssetLoader->LoadMesh("Projectile"),
                   glm::normalize(glm::vec3(sign * id * 0.1f, 1, 0)),
                   10.0,
                   34.0);
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

  
unsigned int ObjectSpaceship::GetWeaponCount() const
{
  assert(_weapons.size() < 0xffff);
  return static_cast<unsigned int>(_weapons.size());
}


bool ObjectSpaceship::FireWeapon(unsigned int weapon_id)
{
  assert(weapon_id < _weapons.size());
  auto weapon = _weapons[weapon_id];
  if(weapon->_heat > 1.0)
    return false;

  auto damage = weapon->_projectile_damage;
  auto bonus = GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE);
  if(bonus->IsActive() > 0.0)
    damage *= bonus->GetValue();

  auto location = glm::vec4(weapon->_location, 1) * glm::toMat4(GetOrientation());
  auto direction = glm::vec4(weapon->_projectile_direction, 1) * glm::toMat4(GetOrientation());
  _scene->AddProjectile(this,
                        GetPosition() + location.xyz(),
                        GetVelocity() * 0.5f + direction.xyz() * static_cast<float>(weapon->_projectile_initial_velocity),
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


void ObjectSpaceship::UpgradeEngines(double power_multiplier)
{
  for(auto e : _engines)
    e->_power *= power_multiplier;
}


void ObjectSpaceship::Hit(double damage, const glm::vec3 & impulse)
{
  auto shield = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
  if(shield->IsActive())
    {
      double reduction = std::min(shield->GetValue(), damage);
      
      shield->Add(-reduction, 0);
     
      damage -= reduction;
    }
  ObjectMovable::Hit(damage, impulse);
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
      auto shader = AssetLoader->LoadShaderProgram("Spaceship");
      shader->Use();
      auto shield = GetUpgrade(SpaceshipUpgrade::Type::SHIELD);
      shader->SetInt("in_shields", shield->IsActive() ? 1 : 0);
      shader->SetFloat("in_time", static_cast<float>(shield->GetTimer() - std::floor(shield->GetTimer())));
      mesh->Draw(model, view, projection, mvp, shader);
    }
}


void ObjectSpaceship::UpgradeFromCollectible(ObjectCollectible * collectible)
{
  for(auto u : _upgrades)
    u->AddFromCollectible(collectible);
}

// todo: remove these two:
void ObjectSpaceship::AddUpgrade(SpaceshipUpgrade::Type type, double value, double time)
{
  auto upgrade = GetUpgrade(type);
  upgrade->Add(value, time);
}
bool ObjectSpaceship::CanAddUpgrade(SpaceshipUpgrade::Type type) const
{
  auto upgrade = GetUpgrade(type);
  return upgrade->CanAdd();
}


