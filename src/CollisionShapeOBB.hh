#ifndef COLLISION_SHAPE_OBB_HH_
#define COLLISION_SHAPE_OBB_HH_

#include "CollisionShape.hh"
#include <vector>


class CollisionShapeOBB : public CollisionShape
{
public:
  CollisionShapeOBB(Object * owner, const glm::vec3 & size, const glm::vec3 & debugcolor = {1.0, 0.5, 0.5});
  
  bool CheckCollisionOnSphere(const CollisionShape & other, glm::vec3 & out_hit_direction) const override;
  bool CheckCollisionOnOBB(const CollisionShape & other, glm::vec3 & out_hit_direction) const override;

  const glm::vec3 & GetSize() const;

private:
  glm::vec3 _size;
  std::vector<glm::vec3> _vertices;

  bool SeparatedByAxis(const CollisionShape & other, const glm::vec3 & axis) const;
  void GetOBBMinMax(const glm::vec3 & axis, float & out_min, float & out_max) const;
};

#endif
