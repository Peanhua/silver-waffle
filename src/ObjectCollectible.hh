#ifndef OBJECT_COLLECTIBLE_HH_
#define OBJECT_COLLECTIBLE_HH_

#include "ObjectMovable.hh"

class ObjectCollectible : public ObjectMovable
{
public:
  ObjectCollectible(Scene * scene, unsigned int score_bonus);

  unsigned int GetScoreBonus() const;
  
private:
  unsigned int _score_bonus;
};

#endif
