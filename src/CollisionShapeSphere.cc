#include "CollisionShapeSphere.hh"
#include "Object.hh"


CollisionShapeSphere::CollisionShapeSphere(Object * owner, double radius)
  : CollisionShape(owner, Type::SPHERE),
    _radius(static_cast<float>(radius))
{
}


bool CollisionShapeSphere::CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const
{
  auto o = static_cast<const CollisionShapeSphere &>(other);
  
  auto distance = glm::distance(GetPosition(), o.GetPosition());
  if(distance > _radius + o._radius)
    return false;

  out_hit_direction = glm::normalize(o.GetPosition() - GetPosition());
  return true;
}


bool CollisionShapeSphere::CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const
{
  auto rv = other.CheckCollisionOnSphere(*this, out_hit_direction);
  out_hit_direction *= -1;
  return rv;
}


float CollisionShapeSphere::GetRadius() const
{
  return _radius;
}
