#ifndef COLLISION_SHAPE_HH_
#define COLLISION_SHAPE_HH_

#include "glm.hh"

class Object;


class CollisionShape
{
public:
  enum class Type
    {
      SPHERE,
      OBB
    };

  CollisionShape(Object * owner, Type type);
  virtual ~CollisionShape();

  const Object * GetOwner() const;
  Type GetType() const;
  bool CheckCollision(const CollisionShape & other, glm::vec3 & out_hit_direction);

  const glm::vec3 & GetPosition() const;

  virtual bool CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const = 0;
  virtual bool CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const = 0;

  
private:
  Object * _owner;
  Type     _type;
};

#endif
