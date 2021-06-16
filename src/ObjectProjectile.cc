#include "ObjectProjectile.hh"
#include "SubsystemAssetLoader.hh"

ObjectProjectile::ObjectProjectile(Scene * scene)
  : ObjectMovable(scene),
    _owner(nullptr)
{
  SetMesh(AssetLoader->LoadMesh("Projectile"));
  SetHealth(0);
  AddToCollisionChannel(CollisionChannel::PROJECTILE);
  AddCollidesWithChannel(CollisionChannel::PLAYER);
  AddCollidesWithChannel(CollisionChannel::ENEMY);
}


void ObjectProjectile::Tick(double deltatime)
{
  ObjectMovable::Tick(deltatime);

  _lifetime_left -= deltatime;
  if(_lifetime_left < 0.0)
    SetHealth(0);
}


void ObjectProjectile::Activate(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime)
{
  assert(!IsAlive());
  SetHealth(1);
  SetPosition(position);
  SetVelocity(velocity);
  _owner = owner;
  _damage = damage;
  _lifetime_left = lifetime;
}


Object * ObjectProjectile::GetOwner() const
{
  return _owner;
}


double ObjectProjectile::GetDamage() const
{
  return _damage;
}

void ObjectProjectile::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  other.Hit(_owner, GetDamage(), -hit_direction);
}
