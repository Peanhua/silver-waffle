#include "ObjectBullet.hh"
#include "MeshCube.hh"

ObjectBullet::ObjectBullet()
  : ObjectMovable(glm::vec3(0, 0, 0))
{
  SetMesh(new MeshCube4());
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
