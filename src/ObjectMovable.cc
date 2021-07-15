#include "ObjectMovable.hh"
#include "Scene.hh"


ObjectMovable::ObjectMovable(Scene * scene, unsigned int random_seed)
  : Object(scene, random_seed),
    _velocity(glm::vec3(0, 0, 0)),
    _angular_velocity(0, 0, 0, 0),
    _angular_velocity_magnitude(0),
    _hit_impulse_enabled(true),
    _is_affected_by_gravity(true),
    _bounciness(-1)
{
  _velocity_enabled[0] =
    _velocity_enabled[1] =
    _velocity_enabled[2] = true;
  AddCollidesWithChannel(CollisionChannel::TERRAIN);
}


void ObjectMovable::Tick(double deltatime)
{
  if(_is_affected_by_gravity)
    {
      auto scene = GetScene();
      if(scene)
        _velocity += scene->GetGravity() * static_cast<float>(deltatime);
    }
    
  Translate(_velocity * static_cast<float>(deltatime));

  if(_angular_velocity_magnitude > 0.01)
    {
      glm::quat src(1, 0, 0, 0);
      glm::quat dst = _angular_velocity;
      glm::quat res = glm::slerp(src, dst, static_cast<float>(_angular_velocity_magnitude * deltatime));
      Rotate(res);
    }
  
  Object::Tick(deltatime);
}


void ObjectMovable::Hit(Object * perpetrator, double damage, const glm::vec3 & impulse)
{
  Object::Hit(perpetrator, damage, impulse);
  if(_hit_impulse_enabled)
    AddImpulse(impulse);
}


void ObjectMovable::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  glm::vec3 bouncing_direction(0, 0, 1); // todo: Fix hit_direction so that it can be used here.

  if(_bounciness > 0.0)
    {
      SetVelocity({0, 0, 0});
      // todo: Maybe also adjust the position so that this object is not inside the other.
      AddImpulse(bouncing_direction * static_cast<float>(_bounciness) * (1.0f + GetRand()));
    }
  else
    Object::OnCollision(other, hit_direction);
}


void ObjectMovable::EnableHitImpulse(bool enabled)
{
  _hit_impulse_enabled = enabled;
}


void ObjectMovable::AddImpulse(const glm::vec3 & impulse)
{
  auto v = _velocity + impulse;
  SetVelocity(v);
}


void ObjectMovable::SetVelocity(const glm::vec3 & velocity)
{
  _velocity = velocity;
  if(!_velocity_enabled[0]) _velocity.x = 0;
  if(!_velocity_enabled[1]) _velocity.y = 0;
  if(!_velocity_enabled[2]) _velocity.z = 0;
}


const glm::vec3 & ObjectMovable::GetVelocity() const
{
  return _velocity;
}


void ObjectMovable::SetAngularVelocity(const glm::quat & angular_velocity, double magnitude)
{
  _angular_velocity = angular_velocity;
  _angular_velocity_magnitude = magnitude;
}


void ObjectMovable::EnableVelocity(bool x, bool y, bool z)
{
  _velocity_enabled[0] = x;
  _velocity_enabled[1] = y;
  _velocity_enabled[2] = z;
}


void ObjectMovable::SetIsAffectedByGravity(bool is_affected)
{
  _is_affected_by_gravity = is_affected;
}


void ObjectMovable::SetBounciness(double bounciness)
{
  _bounciness = bounciness;
}
