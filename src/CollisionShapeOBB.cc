#include "CollisionShapeOBB.hh"
#include "CollisionShapeSphere.hh"
#include "Object.hh"


CollisionShapeOBB::CollisionShapeOBB(Object * owner, const glm::vec3 & size)
  : CollisionShape(owner, Type::OBB),
    _size(size)
{
  auto a = -size;// * 0.5f;
  auto b =  size;// * 0.5f;
  assert(size.x > 0);
  assert(size.y > 0);
  assert(size.z > 0);
  _vertices.push_back(a);
  _vertices.push_back({a.x, a.y, b.z});
  _vertices.push_back({a.x, b.y, a.z});
  _vertices.push_back({a.x, b.y, b.z});
  _vertices.push_back({b.x, a.y, a.z});
  _vertices.push_back({b.x, a.y, b.z});
  _vertices.push_back({b.x, b.y, a.z});
  _vertices.push_back(b);
}


bool CollisionShapeOBB::CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const
{
  auto sphere = dynamic_cast<const CollisionShapeSphere *>(&other);
  assert(sphere);
  
  auto axis = sphere->GetPosition() - GetPosition();
  auto naxis = glm::normalize(axis);

  float max = -9999;
  for(auto vv : _vertices)
    {
      auto v = (glm::vec4(vv, 1) * glm::toMat4(GetOwner()->GetOrientation())).xyz();
      auto projection = glm::dot(v, naxis);
      max = std::max(max, projection);
    }

  // This probably produces slightly incorrect results.
  auto len = glm::length(axis);
  if(len > 0 && len - max > sphere->GetRadius())
    return false;
  
  out_hit_direction = glm::normalize(sphere->GetPosition() - GetPosition());
  return true;
}


bool CollisionShapeOBB::CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const
{ // Uses separating axis theorem.
  auto me = GetOwner();
  auto ot = other.GetOwner();

  if(SeparatedByAxis(other, me->GetForwardVector()))
    return false;
  if(SeparatedByAxis(other, me->GetRightVector()))
    return false;
  if(SeparatedByAxis(other, me->GetUpVector()))
    return false;
  
  if(SeparatedByAxis(other, ot->GetForwardVector()))
    return false;
  if(SeparatedByAxis(other, ot->GetRightVector()))
    return false;
  if(SeparatedByAxis(other, ot->GetUpVector()))
    return false;

  if(SeparatedByAxis(other, glm::cross(me->GetForwardVector(), ot->GetForwardVector())))
    return false;
  if(SeparatedByAxis(other, glm::cross(me->GetForwardVector(), ot->GetRightVector())))
    return false;
  if(SeparatedByAxis(other, glm::cross(me->GetForwardVector(), ot->GetUpVector())))
    return false;

  if(SeparatedByAxis(other, glm::cross(me->GetRightVector(), ot->GetForwardVector())))
    return false;
  if(SeparatedByAxis(other, glm::cross(me->GetRightVector(), ot->GetRightVector())))
    return false;
  if(SeparatedByAxis(other, glm::cross(me->GetRightVector(), ot->GetUpVector())))
    return false;
  
  if(SeparatedByAxis(other, glm::cross(me->GetUpVector(), ot->GetForwardVector())))
    return false;
  if(SeparatedByAxis(other, glm::cross(me->GetUpVector(), ot->GetRightVector())))
    return false;
  if(SeparatedByAxis(other, glm::cross(me->GetUpVector(), ot->GetUpVector())))
    return false;

  out_hit_direction = glm::normalize(ot->GetPosition() - GetPosition());
  return true;
}


bool CollisionShapeOBB::SeparatedByAxis(const CollisionShape & other, const glm::vec3 & axis) const
{
  if(glm::length(axis) == 0)
    return false;

  auto othr = dynamic_cast<const CollisionShapeOBB *>(&other);
  assert(othr);
  
  float mymin, mymax;
  float otmin, otmax;

  GetOBBMinMax(axis, mymin, mymax);
  othr->GetOBBMinMax(axis, otmin, otmax);

  return otmax < mymin || mymax < otmin;
}


void CollisionShapeOBB::GetOBBMinMax(const glm::vec3 & axis, float & out_min, float & out_max) const
{
  out_min = 9999;
  out_max = -9999;
  for(auto vv : _vertices)
    {
      auto v = GetPosition() + (glm::vec4(vv, 1) * glm::toMat4(GetOwner()->GetOrientation())).xyz();
      auto projection = glm::dot(v, axis);
      out_min = std::min(out_min, projection);
      out_max = std::max(out_max, projection);
    }

  assert(out_min < out_max);
}


const glm::vec3 & CollisionShapeOBB::GetSize() const
{
  return _size;
}

