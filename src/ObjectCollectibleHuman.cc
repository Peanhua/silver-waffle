#include "ObjectCollectibleHuman.hh"
#include "CollisionShapeOBB.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


ObjectCollectibleHuman::ObjectCollectibleHuman()
  : ObjectCollectible(nullptr),
    _next_think_time(0),
    _moving_direction(0)
{
  SetBonus(Type::HUMAN, 1);
  SetMesh(AssetLoader->LoadMesh("Human"));
  SetCollisionShape(new CollisionShapeOBB(this, GetMesh()->GetBoundingBoxHalfSize()));
  SetBounciness(1.0);
}


void ObjectCollectibleHuman::Tick(double deltatime)
{
  ObjectCollectible::Tick(deltatime);
  if(!IsAlive())
    return;
  
  _next_think_time -= deltatime;
  if(_next_think_time < 0)
    {
      _next_think_time += 5.0 + 5.0 * static_cast<double>(GetRand());
      _moving_direction = GetRand() < 0.5f ? -1 : 1;
    }  
}


void ObjectCollectibleHuman::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  ObjectCollectible::OnCollision(other, hit_direction);
  if(std::abs(hit_direction.x) < std::abs(hit_direction.z))
    AddImpulse({1.0f * _moving_direction, 0, 0});
}
