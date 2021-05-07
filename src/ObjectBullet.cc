#include "ObjectBullet.hh"
#include "SubsystemAssetLoader.hh"

ObjectBullet::ObjectBullet(Scene * scene)
  : ObjectMovable(scene)
{
  SetMesh(AssetLoader->LoadMesh("PlayerBullet"));
  SetHealth(0);
}


void ObjectBullet::Tick(double deltatime)
{
  if(!IsAlive())
    return;

  ObjectMovable::Tick(deltatime);

  _lifetime_left -= deltatime;
  if(_lifetime_left < 0.0)
    SetHealth(0);
}


void ObjectBullet::Activate(const glm::vec3 & position, const glm::vec3 & velocity, double lifetime)
{
  assert(!IsAlive());
  SetHealth(1);
  SetPosition(position);
  SetVelocity(velocity);
  _lifetime_left = lifetime;
}
