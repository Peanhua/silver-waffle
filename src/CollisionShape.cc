#include "CollisionShape.hh"
#include "Object.hh"


CollisionShape::CollisionShape(Object * owner, Type type)
  : _owner(owner),
    _type(type)
{
  assert(_owner);
}


CollisionShape::~CollisionShape()
{
}


const Object * CollisionShape::GetOwner() const
{
  return _owner;
}


CollisionShape::Type CollisionShape::GetType() const
{
  return _type;
}


bool CollisionShape::CheckCollision(const CollisionShape & other, glm::vec3 & out_hit_direction)
{
  switch(other.GetType())
    {
    case Type::SPHERE:
      return CheckCollisionOnSphere(other, out_hit_direction);
    case Type::OBB:
      return CheckCollisionOnOBB(other, out_hit_direction);
    }
  return false;
}


const glm::vec3 & CollisionShape::GetPosition() const
{
  return _owner->GetPosition();
}
