#ifndef OBJECT_COLLECTIBLE_HH_
#define OBJECT_COLLECTIBLE_HH_

#include "ObjectMovable.hh"
#include <map>


class ObjectCollectible : public ObjectMovable
{
public:
  enum Type
    {
      TYPE_SCORE_BONUS,
      TYPE_DAMAGE_MULTIPLIER,
      TYPE_SCORE_MULTIPLIER,
      TYPE_SHIELD,
      TYPE_SIZEOF,
      TYPE_NONE = TYPE_SIZEOF
    };
  
  ObjectCollectible(Scene * scene);

  void   SetBonus(Type type, double amount);
  bool   HasBonus(Type type) const;
  double GetBonus(Type type) const;
  
private:
  std::map<Type, double> _bonuses;
};

#endif
