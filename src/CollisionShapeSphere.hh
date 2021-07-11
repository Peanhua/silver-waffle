#ifndef COLLISION_SHAPE_SPHERE_HH_
#define COLLISION_SHAPE_SPHERE_HH_

#include "CollisionShape.hh"


class CollisionShapeSphere : public CollisionShape
{
public:
  CollisionShapeSphere(Object * owner, double radius);
  
  bool CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const override;
  bool CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const override;

  float GetRadius() const;
  
private:
  float _radius;
};

#endif
