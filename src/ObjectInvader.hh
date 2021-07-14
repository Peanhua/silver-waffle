#ifndef OBJECT_INVADER_HH_
#define OBJECT_INVADER_HH_

#include "ObjectSpaceship.hh"
#include "CollisionShape.hh"


class ObjectInvader : public ObjectSpaceship
{
public:
  ObjectInvader(Scene * scene, unsigned int random_seed);

  void OnDestroyed(Object * destroyer) override;
  void SetInvaderType(unsigned int type);
  void CreateCollisionShape(CollisionShape::Type type);
};

#endif
